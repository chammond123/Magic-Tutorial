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

int Zone::getCount() { return cards.count(); }

void Zone::shuffle() {
    // std::shuffle(cards.first(), cards.last(), randomEngine);
    // Create a temporary vector and copy cards
    QVector<Card*> tempVector = cards;
    cards.clear();

    // Fisher-Yates shuffle algorithm
    while (!tempVector.isEmpty()) {
        // Generate a random index
        std::uniform_int_distribution<int> dist(0, tempVector.size() - 1);
        int randomIndex = dist(randomEngine);

        // Move the randomly selected card to our original vector
        cards.append(tempVector.takeAt(randomIndex));
    }
}

bool Zone::findCard(Card* card) {
    return cards.contains(card);
}

Card* Zone::drawTop() {
    if (cards.isEmpty()){
        return nullptr;
    }
    return cards.first();
}

void Zone::addCard(Card* card, bool onTop) {
    if (onTop){
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
    // for (Card* cardptr : cards) {
    //     delete cardptr;
    // }
    cards.clear();
}

