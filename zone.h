#ifndef ZONE_H
#define ZONE_H

#include "card.h"
#include <random>

class Zone
{
private:
    QVector<Card*> cards;
    bool isFaceUp;

    unsigned seed;
    std::default_random_engine randomEngine;

public:
    /**
     * @brief Default Zone Constructor
     */
    Zone();

    /**
     * @brief intialize
     * @param initialCards
     * Allows for a reinitialization of a cards in a zone
     */
    void intialize(const QVector<Card> &initialCards);

    /**
     * @brief setVisibility
     * @param visibility
     * Sets a zone's visibility state
     */
    void setVisibility(bool visibility);

    /**
     * @brief getVisibility
     * @return 
     * Get's a zone's visibility state for rendering on the GUI
     */
    bool getVisibility();

    /**
     * @brief shuffle
     */
    void shuffle();

    /**
     * @brief drawCard
     */
    void drawCard();
};

#endif // ZONE_H
