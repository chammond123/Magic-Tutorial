#include "zone.h"
#include "card.h"
#include "cardDictionary.h"
#include <algorithm>
#include <chrono>

Zone::Zone() {

    // Initialize random engine
    seed = std::chrono::system_clock::now().time_since_epoch().count();
    randomEngine = std::default_random_engine(seed);
}

void Zone::setVisibility(bool visibility) {
    isFaceUp = visibility;
}

iterator Zone::begin() { return cards.begin(); }

iterator Zone::end() { return cards.end(); }

int Zone::getCount() { return count; }

void Zone::shuffle() {
    std::shuffle(cards.first(), cards.last(), randomEngine);
}

bool Zone::findCard(Card* card) {
    return cards.contains(card);
}

Card* Zone::drawTop() {
    return cards.last();
}

void Zone::addCard(Card* card, bool onTop) {
    if (!onTop){
        cards.prepend(card);
    }
    else{
        cards.append(card);
    }
}

void Zone::removeCard(Card* card){
    cards.removeOne(card);
}

Zone::~Zone() {
    for (Card* cardptr : cards) {
        delete cardptr;
    }
    cards.clear();
}

