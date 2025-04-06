#include "player.h"
#include "type.h"

using enum ManaColor;

Player::Player(QWidget *parent)
    : QWidget{parent}
{
    manaPool[Red] = 0;
    manaPool[Blue] = 0;
    manaPool[White] = 0;
    manaPool[Green] = 0;
    manaPool[Black] = 0;

}

void Player::gainLife(int amount){
    health += amount;
    emit healthChanged(health);
}

void Player::takeDamage(int amount){
    health -= amount;
    emit healthChanged(health);
}

void Player::addMana(int amount, ManaColor color){
    manaPool[color] += amount;
    emit manaPoolChanged();
}

void Player::useMana(int amount, ManaColor color){
    manaPool[color] -= amount;
    emit manaPoolChanged();
}

void Player::drawCard(int amount){
    // Check to see if any cards left
    for (int i = 0; i < amount; i++){
        if(false){ // Library.isEmpty()){     WILL ADD LATER WHEN DECK IS IMPLEMENTED
            emit playerLost();
            return;
        }

        Card* card = Library.drawTopCard();
        Hand.append(card);
        emit cardDrawn(card);
        emit handChanged();
        emit libraryChanged();
    }
}

void Player::moveCard(int cardIndex, QString sourceZone, QString targetZone){
    Card* card = findCardInZone(cardIndex, sourceZone);

    QVector<Card*>* source = nullptr;
    QVector<Card*>* target = nullptr;

    // Map pointers to actual targets
    if(sourceZone == "hand") source = &Hand;
    if(sourceZone == "battlefield") source = &Battlefield;
    if(sourceZone == "graveyard") source = &Graveyard;
    if(sourceZone == "exile") source = &Exile;

    if(targetZone == "hand") target = &Hand;
    if(targetZone == "battlefield") target = &Battlefield;
    if(targetZone == "graveyard") target = &Graveyard;
    if(targetZone == "exile") target = &Exile;

    if(!source || !target || !source->contains(card)){
        emit invalidAction("Invalid Card movement");
        return;
    }

    source->removeOne(card);
    target->append(card);

    updateAllUI();
}

void Player::mill(int amount){
    for(int i = 0; i < amount; i++){
        if(false){ // Library.isEmpty()){     WILL ADD LATER WHEN DECK IS IMPLEMENTED
            emit playerLost();
            return;
        }
        Card* card = Library.drawTopCard();
        Graveyard.append(card);
        emit libraryChanged();
        emit graveyardChanged();
    }
}

void Player::playCard(int index, QString zone){

    if (index < 0 || index >= Hand.size()){
        emit invalidAction("Card selected out of bounds");
        return;
    }
    Card* card = findCardInZone(index, zone);

    if(card->isLand()){
        Hand.removeAt(index);
        Battlefield.append(card);

        emit cardPlayed(card);
        emit handChanged();
        emit battlefieldChanged();
    }
    else{
        if(canPayMana(card->getManaCost())){ // Need this function from Card
            payMana(card->getManaCost());
            Hand.removeAt(index);

            if(card->isPermanent()){ // Need this function from Card class
                Battlefield.append(card);
                emit battlefieldChanged();
            }
            else{
                // Instant / Sorcery Card
                card->DoAction(this);
                Graveyard.append(card);
                emit graveyardChanged();
            }

            emit cardPlayed(card);
            emit handChanged();
            emit manaPoolChanged();
        }
    }
}

void Player::updateAllUI(){
    emit handChanged();
    emit manaPoolChanged();
    emit battlefieldChanged();
    emit graveyardChanged();
    emit exileChanged();
    emit healthChanged(health);
}

void Player::untapPhase(){
    for (Card* card : Battlefield){
        card->untap();
    }
    emit battlefieldChanged();
}

void Player::upkeepStep(){
    for(Card* card : Battlefield){
        card->triggerUpkeep();
    }
}

void Player::endTurn(){
    if (Hand.size() <= 7){
        emit turnEnded();
        return;
    }
    emit requestDiscard("Hand");    // TODO: clarify zone
}
