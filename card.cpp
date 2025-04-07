#include "card.h"

Card::Card() {

}

QString Card::getName() const { return name; }
QString Card::getDescription() const { return description; }
QImage Card::getImage() const { return image; }
ManaType Card::getColor() const { return color; }
Ability Card::getAbility() const { return ability; }
CardType Card::getType() const { return type; }
int Card::getToughness() const { return toughness; }
int Card::getPower() const { return power; }
QVector<ManaType> Card::getCost() const { return cost; }
QVector<PropertyType> Card::getKeywords() const { return keywords; }

bool Card::isPermanent() const {
    return (type == CardType::CREATURE ||
            type == CardType::ARTIFACT ||
            type == CardType::ENCHANTMENT ||
            type == CardType::PLANESWALKER ||
            type == CardType::ENCHANTMENT ||
            type == CardType::LAND);
}

void Card::useAbility(){
    // ability.use();
}

bool Card::canBePlayed(const QVector<ManaType> &availableMana) const{
    for (ManaType requiredMana : cost){
        if(std::count(cost.begin(), cost.end(), requiredMana) >
            std::count(availableMana.begin(), availableMana.end(), requiredMana)){
            return false;
        }
    }
    return true;
}

bool Card::hasKeyword(PropertyType keyword) const{
    if(keywords.contains(keyword)){
        return true;
    }
    return false;
}

// Card* Card::clone() const{
//     return;
// }
