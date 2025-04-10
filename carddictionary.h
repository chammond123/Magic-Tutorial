#ifndef CARDDICTIONARY_H
#define CARDDICTIONARY_H
#include <map>
#include <QString>
#include "card.h"
class carddictionary
{
private:
    // Private constructor prevents instantiation
    carddictionary() = delete;

public:
    static std::map<QString, Card> dict;
    static void addCard(Card card);
    static Card getCard(QString cardName);
};
#endif // CARDDICTIONARY_H
