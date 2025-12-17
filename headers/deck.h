#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <vector>
#include <string>

/**
 * @brief The Deck class
 */
class Deck
{
private:

    std::vector<Card*> cards;
public:
    /**
     * @brief Deck Constructor that instantiates all cards
     * @param deckList
     */
    explicit Deck(std::vector<std::string> deckList);

    std::vector<Card*> getCardObjects();

    ~Deck();
};

#endif // DECK_H
