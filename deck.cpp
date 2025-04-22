#include "deck.h"
#include "carddictionary.h"
#include <QFile>

Deck::Deck(QStringList deckList, QObject *parent)
    // : QObject{parent}, fileName{fileName}
{
    for (const auto &cardName : deckList) {
        Card* card = new Card(cardDictionary::getCard(cardName));
        cards.append(card);
    }
}

QVector<Card*> Deck::getCardObjects() {
    return this->cards;
}

Deck::~Deck(){
    for (Card* card : cards){
        delete card;
    }
    cards.clear();
}
