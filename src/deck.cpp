#include "deck.h"
#include "carddictionary.h"

Deck::Deck(std::vector<std::string> deckList)
{
    for (const auto &cardName : deckList) {
        Card* card = new Card(cardDictionary::getCard(cardName));
        cards.push_back(card);
    }
}

std::vector<Card*> Deck::getCardObjects() {
    return this->cards;
}

Deck::~Deck(){
    for (Card* card : cards){
        delete card;
    }
    cards.clear();
}
