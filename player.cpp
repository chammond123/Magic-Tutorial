#include "player.h"
#include "type.h"

Player::Player(QObject *parent)
    : QObject{parent}
{
    manaPool[ManaColor::Red] = 0;
    manaPool[ManaColor::Blue] = 0;
    manaPool[ManaColor::White] = 0;
    manaPool[ManaColor::Green] = 0;
    manaPool[ManaColor::Black] = 0;

    health = 20;

}

void Player::gainLife(int amount){
    qDebug() << "Gain Life called with " << amount;
    health += amount;
    qDebug() << "New Health: " << health;
    emit healthChanged(health);
}

void Player::takeDamage(int amount){
    qDebug() << "Take Damage called with " << amount;
    health -= amount;
    qDebug() << "New Health: " << health;
    emit healthChanged(health);
}

void Player::addMana(QMap<ManaColor, int>* manaCosts){
    for(auto [color, amount] : manaCosts->toStdMap()){
        manaPool[color] += amount;
    }

    emit manaPoolChanged(&manaPool);
}

void Player::useMana(QMap<ManaColor, int>* manaCosts){
    for(auto [color, amount] : manaCosts->toStdMap()){
        manaPool[color] -= amount;
    }

    emit manaPoolChanged(&manaPool);
}

void Player::drawCard(int amount){
    // Check to see if any cards left
    for (int i = 0; i < amount; i++){
        if(false){ // Library.isEmpty()){     TODO: WILL ADD LATER WHEN DECK IS IMPLEMENTED
            emit playerLost();
            return;
        }

        Card* card = nullptr; //Library.drawTopCard(); TODO: ADD AFTER DECK
        Hand.append(card);
        emit cardDrawn(card);
        emit handChanged();
        emit libraryChanged();
    }
}

void Player::moveCard(int cardIndex, QString sourceZone, QString targetZone){
    Card* card = nullptr; // TODO: Implement after card class findCardInZone(cardIndex, sourceZone);

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
        if(false){ // Library.isEmpty()){   TODO:  WILL ADD LATER WHEN DECK IS IMPLEMENTED
            emit playerLost();
            return;
        }
        Card* card = nullptr; // Library.drawTopCard(); TODO: ADD AFTER DECK IS IMPLEMENTED
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
    // Card* card = findCardInZone(index, zone);

    // TODO: Implement after Card has IsLand and isPermanent, as well as mana costs
    // if(card->isLand()){
    //     Hand.removeAt(index);
    //     Battlefield.append(card);

    //     emit cardPlayed(card);
    //     emit handChanged();
    //     emit battlefieldChanged();
    // }
    // else{
    //     if(canPayMana(card->getManaCost())){ // Need this function from Card
    //         payMana(card->getManaCost());
    //         Hand.removeAt(index);

    //         if(card->isPermanent()){ // Need this function from Card class
    //             Battlefield.append(card);
    //             emit battlefieldChanged();
    //         }
    //         else{
    //             // Instant / Sorcery Card
    //             card->DoAction(this);
    //             Graveyard.append(card);
    //             emit graveyardChanged();
    //         }

    //         emit cardPlayed(card);
    //         emit handChanged();
    //         emit manaPoolChanged();
    //     }
    // }
}

void Player::updateAllUI(){
    emit handChanged();
    emit manaPoolChanged(&manaPool);
    emit battlefieldChanged();
    emit graveyardChanged();
    emit exileChanged();
    emit healthChanged(health);
}

// TODO: Have Cards "Untapable and Tapable"
// void Player::untapPhase(){
//     for (Card* card : Battlefield){
//         card->untap();
//     }
//     emit battlefieldChanged();
// }

// TODO: Have Cards have an "Upkeep" function
// void Player::upkeepStep(){
//     for(Card* card : Battlefield){
//         card->triggerUpkeep();
//     }
// }

void Player::endTurn(){
    if (Hand.size() <= 7){
        emit turnEnded();
        return;
    }
    emit requestDiscard("Hand");    // TODO: clarify zone
}
