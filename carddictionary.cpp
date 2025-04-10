// In carddictionary.cpp
#include "carddictionary.h"

// Definition of the static member
std::map<QString, Card> carddictionary::dict;

// Then implement your methods
void carddictionary::addCard(Card card) {
    dict[card.name] = card;
}

Card carddictionary::getCard(QString cardName) {
    return dict[cardName];
}
