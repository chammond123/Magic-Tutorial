#ifndef ZONE_H
#define ZONE_H

#include "card.h"
#include <QIterator>
#include <random>
using iterator = QVector<Card*>::Iterator;

class Zone
{
private:
    /**
     * @brief cards
     * First element represents bottom of card stack
     * Last element represents top of card stack
     */
    QVector<Card*> cards;
    int count = cards.count();


    bool isFaceUp;

    unsigned seed;
    std::default_random_engine randomEngine;

public:
    /**
     * @brief Default Zone Constructor
     * Initializes an empty zone
     */
    Zone();

    /**
     * @brief Overloaded Zone Constructor
     * @param initialCards
     * Initializes a zone with a
     */
    Zone(const QVector<Card*> &initialCards);

    /**
     * @brief Overloaded Zone Constructor
     * @param fileName
     * Initializes a zone by grabbing a file name which
     * contains JSON for card data
     */
    Zone(std::string fileName);

    /**
     * @brief intialize
     * @param initialCards
     * Allows for a reinitialization of a cards in a zone
     */
    void intialize(const QVector<Card*> &initialCards);

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
     * @brief Begin
     * @return
     * Makes Zone class iterable
     */
    QVector<Card*>::iterator begin();

    /**
     * @brief End
     * @return
     * Makes Zone class iterable
     */
    QVector<Card*>::iterator end();

    /**
     * @brief getCount
     * @return
     */
    int getCount();

    /**
     * @brief shuffle
     */
    void shuffle();

    /**
     * @brief findCard
     */
    bool findCard(Card* card);

    /**
     * @brief drawCard
     */
    Card* drawCard();

    /**
     * @brief removeCard
     */
    void removeCard(Card*);

    /**
     * @brief addCard
     */
    void addCard(Card*, bool onTop);

    /**
     * @brief drawTop
     * @return
     */
    Card* drawTop();

    /**
     * @brief Destructor
     */
    ~Zone();
};

#endif // ZONE_H
