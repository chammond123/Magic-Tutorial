#include "card.h"
#include <algorithm>

Card::Card() {}

Card::Card(std::string name){
    this->name = name;
    isLand = false;
    isTapped = false;
    isPermanent = false;
    hasSummoningSickness = false;
    toughness = -1;
    power = -1;
}

Card::Card(const Card& other) :
    name(other.name),
    description(other.description),
    color(other.color),
    ability(other.ability),
    type(other.type),
    toughness(other.toughness),
    power(other.power),
    cost(other.cost),
    keywords(other.keywords),
    isTapped(other.isTapped),
    isPermanent(other.isPermanent),
    isLand(other.isLand),
    hasSummoningSickness(other.hasSummoningSickness),
    flavorText(other.flavorText),
    needsTarget(other.needsTarget)
{}

Card& Card::operator=(const Card& other) {
    if (this != &other) {
        name = other.name;
        description = other.description;
        color = other.color;
        ability = other.ability;
        type = other.type;
        toughness = other.toughness;
        power = other.power;
        cost = other.cost;
        keywords = other.keywords;
        isTapped = other.isTapped;
        isPermanent = other.isPermanent;
        isLand = other.isLand;
        needsTarget = other.needsTarget;
        flavorText = other.flavorText;
    }
    return *this;
}

bool Card::hasKeyword(PropertyType keyword) const
{
    return std::find(keywords.begin(), keywords.end(), keyword) != keywords.end();
}

void Card::takeDamage(int damage){
    currHealth -= damage;
}

void Card::gainHealth(int health){
    currHealth += health;
}

void Card::cleanupCard(){
    currHealth = toughness;
}

void Card::setAbility(Ability a){
    ability = a;
}
