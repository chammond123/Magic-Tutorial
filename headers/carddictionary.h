#ifndef CARDDICTIONARY_H
#define CARDDICTIONARY_H
#include <map>
#include <QString>
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
    static std::map<QString, Card> dict;
    static void addCard(Card card);
    static Card getCard(QString cardName);
};
#endif // CARDDICTIONARY_H
