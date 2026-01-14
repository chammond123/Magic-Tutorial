#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>
#include <map>
#include "type.h"
#include <algorithm>
#include "ability.h"

class Card
{
public:
    Card();
    Card(const Card& other);
    Card& operator=(const Card& other);
    //For testing
    Card(std::string name);

    bool canBePlayed(const std::vector<ManaType> &availableMana) const;
    bool hasKeyword(PropertyType keyword) const;

    bool shouldEnable;

    //take damage
    void takeDamage(int damage);
    //gain health
    void gainHealth(int health);
    //reset health
    void cleanupCard();

    void setAbility(Ability a);

public:
    int currHealth = toughness;
    std::string name;
    std::string description;
    // QImage removed - image handling needs separate implementation
    ManaType color;
    Ability ability = Ability();
    CardType type;
    int toughness;
    int power;
    std::map<ManaType, int> cost;
    std::vector<PropertyType> keywords;
    bool isTapped;
    bool isPermanent;
    bool isLand;
    bool hasSummoningSickness = true;
    bool isCountered = false;
    std::string flavorText;
    bool needsTarget = false;
    bool isOnStack = false;
};

#endif // CARD_H
