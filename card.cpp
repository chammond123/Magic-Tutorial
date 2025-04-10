#include "card.h"

Card::Card() {}

Card::Card(const Card& other) :
    name(other.name),
    description(other.description),
    image(other.image),
    color(other.color),
    ability(other.ability),
    type(other.type),
    toughness(other.toughness),
    power(other.power),
    cost(other.cost),
    keywords(other.keywords),
    isTapped(other.isTapped),
    isPermanent(other.isPermanent)
{}

bool Card::hasKeyword(PropertyType keyword) const
{
    if (keywords.contains(keyword)) {
        return true;
    }
    return false;
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
