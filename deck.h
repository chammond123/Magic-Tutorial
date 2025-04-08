#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <random>

class Deck
{
private:
    QVector<Card> cards;

    bool isFaceUp;

    unsigned seed;
    std::default_random_engine randomEngine;

public:
    /**
     * @brief Deck Constructor
     */
    Deck();

    /**
     * @brief intialize
     * @param initialCards
     * Allows for a reinitialization of a cards in a deck
     */
    void intialize(const QVector<Card> &initialCards);

    /**
     * @brief shuffle
     */
    void shuffle();

    /**
     * @brief drawCard
     */
    void drawCard();
};

#endif // DECK_H
