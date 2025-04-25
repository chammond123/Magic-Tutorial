#include "player.h"
#include "type.h"
#include "zone.h"

Player::Player(QStringList deckList, QObject *parent)
    : QObject{parent},
    deck(deckList)
{
    hasSummoningSickness = true;
    hasPlayedLand = false;
    hasntDrawnForTurn = false;
    madeAction = false;
    playerID = 0;
    Library.type = ZoneType::LIBRARY;
    Graveyard.type = ZoneType::GRAVEYARD;
    Battlefield.type = ZoneType::BATTLEFIELD;
    Exile.type = ZoneType::EXILE;
    Hand.type = ZoneType::HAND;

    manaPool[ManaType::RED] = 0;
    manaPool[ManaType::BLUE] = 0;
    manaPool[ManaType::WHITE] = 0;
    manaPool[ManaType::GREEN] = 0;
    manaPool[ManaType::BLACK] = 0;

    health = 20;

    // Initializes card pointers in deck zone
    for (Card* card : deck.getCardObjects()) {
        Library.addCard(card, true);
    }
    Library.shuffle();

    drawCard(7);
}

void Player::gainLife(int amount)
{
    health += amount;
}

void Player::takeDamage(int amount)
{
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
}

void Player::useMana(Card* card)
{
    QMap<ManaType, int> manaCosts = card->cost;

    if (manaCosts[ManaType::ANY] > 0){
        for(auto [color, amount] : selectedMana.toStdMap()){
            manaPool[color] -= amount;
        }
    }

    for (auto [color, amount] : manaCosts.toStdMap()) {
        if(color == ManaType::ANY){
            continue;
        }
        manaPool[color] -= amount;
    }


}

void Player::drawCard(int amount)
{
    // Check to see if any cards left
    for (int i = 0; i < amount; i++) {
        if ( Library.getCount() <= 0){
            emit playerLost();
            return;
        }

        Card* card = Library.drawTop();
        moveCardZone(card, Library, Hand, false);
        emit cardDrawn(card);
    }
}

void Player::moveCardString(Card *card, QString sourceString, QString targetString, bool OnTop)
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

    source->removeCard(card);
    target->addCard(card, OnTop);
}

void Player::moveCardZone(Card *card, Zone&  sourceZone, Zone& targetZone, bool OnTop)
{
    sourceZone.removeCard(card);
    targetZone.addCard(card, OnTop);
}

void Player::mill(int amount)
{
    for (int i = 0; i < amount; i++) {
        if (Library.getCount() <= 0){
            emit playerLost();
            return;
        }
        Card *card = Library.drawTop();
        Graveyard.addCard(card, true);
    }
}

void Player::playCard(Card* card)
{
    if(card->isLand){
        moveCardZone(card, Hand, Battlefield, false);
    }
    else{
        if(canPayMana(card)){ // Need this function from Card
            useMana(card);

            if(card->isPermanent){ // Need this function from Card class
                moveCardZone(card, Hand, Battlefield, false);
            }
            else{
                // Instant / Sorcery Card
                // card->ability.activate();
                moveCardZone(card, Hand, Graveyard, true);
            }
        }
    }
}

bool Player::canPayMana(Card* card)
{
    QMap<ManaType, int> manaCosts = card->cost;
    int costTotal = 0;
    int totalMana = 0;

    for (auto [color, value] : manaCosts.toStdMap()) {
        if (color == ManaType::ANY){
            continue;
        }
        if (value > manaPool[color]) {
            return false;
        }
        costTotal += value;
    }

    for (int value : manaPool){
        totalMana += value;
    }

    if (costTotal > totalMana){
        return false;
    }

    return true;
}

void Player::onBlockRequested(Card *attacker, Card *defender)
{
    int damage = attacker->power;
    if (defender == nullptr) {
        takeDamage(damage);
        return;
    }
    int toughness = defender->toughness;
    if (toughness > damage) {
        // Emit something to let gamemanager know the attack failed.
        return;
    } else {
        moveCardZone(defender, Battlefield, Graveyard, true);
    }
}

void Player::resolveCard(Card* card) {
    // This gets called by GameManager when spell resolves
    useMana(card);

    if (card->isPermanent) {
        moveCardZone(card, Hand, Battlefield, false);
    } else {
        // Instant/Sorcery
        // card->useAbility();
        moveCardZone(card, Hand, Graveyard, true);
    }
}

void Player::tapCard(Card* card){
    card->isTapped = true;
}

void Player::untap(){
    hasPlayedLand = false;
    for (Card* card : Battlefield){
        card->isTapped = false;
    }
}


void Player::upkeepPhase(){
    for(Card* card : Battlefield){
        card->hasSummoningSickness = false;
    }
}

void Player::cleanupPhase(){
    for(Card* card : Battlefield){
        card->cleanupCard();
    }
}

void Player::emptyManaPool(){
    for(ManaType color : manaPool.keys()){
        manaPool[color] = 0;
    }
}

void Player::endStepPhase(){
    for(Card* card : Battlefield){
        // TODO: Update this
    }
}

void Player::endTurn()
{
    if (Hand.getCount() > 7) {
        emit requestDiscard("Hand");
    }
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

QVector<Zone*> Player::getZones(){
    QVector<Zone*> zones;
    zones.append(&Library);
    zones.append(&Graveyard);
    zones.append(&Battlefield);
    zones.append(&Exile);
    zones.append(&Hand);

    return zones;
}
