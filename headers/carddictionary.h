#ifndef CARDDICTIONARY_H
#define CARDDICTIONARY_H
#include <map>
#include <string>
#include "card.h"

/**
 * @brief The cardDictionary class
 */
class cardDictionary
{
private:
    // Private constructor prevents instantiation
    cardDictionary() = delete;
    static void giveAbility(Card* c);

public:
    static std::map<std::string, Card> dict;
    static void addCard(Card card);
    static Card getCard(std::string cardName);
};
#endif // CARDDICTIONARY_H
