#include "zone.h"
#include "card.h"
#include <chrono>
#include <algorithm>

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

int Zone::getCount() { return cards.size(); }

void Zone::shuffle() {
    std::vector<Card*> tempVector = cards;
    cards.clear();

    // Fisher-Yates shuffle algorithm
    while (!tempVector.empty()) {
        // Generate a random index
        std::uniform_int_distribution<int> dist(0, tempVector.size() - 1);
        int randomIndex = dist(randomEngine);

        // Move the randomly selected card to our original vector
        cards.push_back(tempVector[randomIndex]);
        tempVector.erase(tempVector.begin() + randomIndex);
    }
}

bool Zone::findCard(Card* card) {
    return std::find(cards.begin(), cards.end(), card) != cards.end();
}

bool Zone::containsEnabledType(CardType t){
    for (Card* c : cards){
        if (c->type == t && c->shouldEnable){
            return true;
        }
    }
    return false;
}

Card* Zone::drawTop() {
    if (cards.empty()){
        return nullptr;
    }
    return cards.front();
}

void Zone::addCard(Card* card, bool onTop) {
    if (onTop){
        cards.insert(cards.begin(), card);
    }
    else{
        cards.push_back(card);
    }
}

void Zone::removeCard(Card* card){
    auto it = std::find(cards.begin(), cards.end(), card);
    if (it != cards.end()) {
        cards.erase(it);
    }
}

Zone::~Zone() {
    cards.clear();
}

