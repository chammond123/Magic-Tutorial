#include "player.h"
#include "type.h"
#include "zone.h"

Player::Player(QObject *parent)
    : QObject{parent}
{
    manaPool[ManaType::RED] = 0;
    manaPool[ManaType::BLUE] = 0;
    manaPool[ManaType::WHITE] = 0;
    manaPool[ManaType::GREEN] = 0;
    manaPool[ManaType::BLACK] = 0;

    health = 20;

    drawCard(7);
}

void Player::gainLife(int amount)
{
    qDebug() << "Gain Life called with " << amount;
    health += amount;
    qDebug() << "New Health: " << health;
    emit healthChanged(health);
}

void Player::takeDamage(int amount)
{
    qDebug() << "Take Damage called with " << amount;
    health -= amount;
    if (health <= 0){
        emit playerLost();
    }
}

void Player::addMana(QMap<ManaType, int> *manaCosts)
{
    for (auto [color, amount] : manaCosts->toStdMap()) {
        manaPool[color] += amount;
    }

    emit manaPoolChanged(&manaPool);
}

void Player::useMana(QMap<ManaType, int> *manaCosts)
{
    for (auto [color, amount] : manaCosts->toStdMap()) {
        manaPool[color] -= amount;
    }

    emit manaPoolChanged(&manaPool);
}

void Player::drawCard(int amount)
{
    // Check to see if any cards left
    for (int i = 0; i < amount; i++) {
        if ( Library.getCount() > 0){
            emit playerLost();
            return;
        }

        Card *card = Library.drawTop();
        Hand.addCard(card);
        emit cardDrawn(card);
        emit handChanged();
        emit libraryChanged();
    }
}

void Player::moveCard(Card *card, QString sourceString, QString targetString)
{
    Zone *source = nullptr;
    Zone *target = nullptr;

    QString sourceZone = sourceString.toLower();
    QString targetZone = targetString.toLower();

    // Map pointers to actual targets
    if (sourceZone == "hand")
        source = &Hand;
    if (sourceZone == "battlefield")
        source = &Battlefield;
    if (sourceZone == "graveyard")
        source = &Graveyard;
    if (sourceZone == "exile")
        source = &Exile;

    if (targetZone == "hand")
        target = &Hand;
    if (targetZone == "battlefield")
        target = &Battlefield;
    if (targetZone == "graveyard")
        target = &Graveyard;
    if (targetZone == "exile")
        target = &Exile;

    if (!source || !target || !source->findCard(card)) {
        emit invalidAction("Invalid Card movement");
        return;
    }

    source->removeCard(card, false);
    target->addCard(card);

    updateAllUI();
}

void Player::mill(int amount)
{
    for (int i = 0; i < amount; i++) {
        if (Library.getCount() > 0){
            emit playerLost();
            return;
        }
        Card *card = Library.drawTop();
        Graveyard.addCard(card);
        emit libraryChanged();
        emit graveyardChanged();
    }
}

void Player::playCard(Card* card)
{
    // TODO: Implement after Card has IsLand and isPermanent, as well as mana costs
    if(card->isLand()){
        Hand.removeCard(card, false);
        Battlefield.addCard(card);

        emit cardPlayed(card);
        emit handChanged();
        emit battlefieldChanged();
    }
    else{
        if(canPayMana(card->getManaCost())){ // Need this function from Card
            payMana(card->getManaCost());
            Hand.removeCard(card, false);

            if(card->isPermanent()){ // Need this function from Card class
                Battlefield.addCard(card);
            }
            else{
                // Instant / Sorcery Card
                card->useAbility();
                Graveyard.addCard(card);
            }
        }
    }
}

bool Player::canPayMana(Card* card)
{
    QMap<ManaType, int> manaCosts = card->getCost();
    for (auto [color, value] : manaCosts.toStdMap()) {
        if (value > manaPool[color]) {
            return false;
        }
    }

    return true;
}

void Player::onBlockRequested(Card *attacker, Card *defender)
{
    int damage = attacker->getPower();
    if (defender == nullptr) {
        takeDamage(damage);
    }
    int toughness = defender->getToughness();
    if (toughness > damage) {
        // Emit something to let gamemanager know the attack failed.
        return;
    } else {
        moveCard(defender, "battlefield", "graveyard");
    }
}

void Player::updateAllUI()
{
    emit handChanged();
    emit manaPoolChanged(&manaPool);
    emit battlefieldChanged();
    emit graveyardChanged();
    emit exileChanged();
    emit healthChanged(health);
}

// TODO: Have Cards "Untapable and Tapable"
void Player::untapPhase(){
    for (Card* card : Battlefield){
        card->tapped = false;
    }
    emit battlefieldChanged();
}

// TODO: Have Cards have an "Upkeep" function
void Player::upkeepPhase(){
    for(Card* card : Battlefield){
        card->triggerUpkeep();
    }
}

void Player::cleanUpPhase(){
    for(Card* card : Battlefield){
        card->currHealth = maxHealth;
    }
}

void Player::endTurn()
{
    if (Hand.getCount() <= 7) {
        emit turnEnded();
        return;
    }
    emit requestDiscard("Hand"); // TODO: clarify zone
}

Zone* Player::findCardZone(Card* card)
{
    if (Hand.findCard(card)) {
        return &Hand;
    } else if (Battlefield.findCard(card)) {
        return &Battlefield;
    } else if (Graveyard.findCard(card)) {
        return &Graveyard;
    } else if (Exile.findCard(card)) {
        return &Exile;
    } else if (Library.findCard(card)){
        // Library is special since you typically can't directly access cards
        // This is just for UI debugging or special effects
        return &Library;
    }
    return nullptr;
}
