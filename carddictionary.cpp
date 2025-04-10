// In carddictionary.cpp
#include "carddictionary.h"

// Definition of the static member
std::map<QString, Card> cardDictionary::dict;

void cardDictionary::addCard(Card card) {
    dict[card.name] = card;
}

Card cardDictionary::getCard(QString cardName) {
    return dict[cardName];
}
