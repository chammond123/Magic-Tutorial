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

// QString Card::getName() const { return name; }
// QString Card::getDescription() const { return description; }
// QImage Card::getImage() const { return image; }
// ManaType Card::getColor() const { return color; }
// Ability Card::getAbility() const { return ability; }
// CardType Card::getType() const { return type; }
// int Card::getToughness() const { return toughness; }
// int Card::getPower() const { return power; }
// QVector<ManaType> Card::getCost() const { return cost; }
// QVector<PropertyType> Card::getKeywords() const { return keywords; }

// bool Card::isLand() const{
//     return type == CardType::LAND;
// }

// bool Card::isPermanent() const {
//     return (type == CardType::CREATURE ||
//             type == CardType::ARTIFACT ||
//             type == CardType::ENCHANTMENT ||
//             type == CardType::PLANESWALKER ||
//             type == CardType::ENCHANTMENT ||
//             type == CardType::LAND);
// }

// bool Card::canBePlayed(const QVector<ManaType> &availableMana) const{
//     for (ManaType requiredMana : cost){
//         if(std::count(cost.begin(), cost.end(), requiredMana) >
//             std::count(availableMana.begin(), availableMana.end(), requiredMana)){
//             return false;
//         }
//     }
//     return true;
// }

bool Card::hasKeyword(PropertyType keyword) const
{
    if (keywords.contains(keyword)) {
        return true;
    }
    return false;
}
