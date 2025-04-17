#include "ability.h"
#include "card.h"
#include "player.h"

// Constructor to initialize ability with type, amount, and effect
Ability::Ability(abilityType type, int amount, std::function<void(Player*, Card*)> eff)
    : type(type), amount(amount), effect(eff) {}

// Implementation for using ability on a Player
void Ability::use(Player* player) {
    if (effect && player) {
        effect(player, nullptr);
    }
}

// Implementation for using ability on a Card
void Ability::use(Card* card) {
    if (effect && card) {
        effect(nullptr, card);
    }
}

// Static methods to create abilities

Ability Ability::damageTarget(int amount) {
    return Ability(abilityType::DAMAGE, amount, [amount](Player* p, Card* c){
        if (p) {
            p->takeDamage(amount);  // Apply damage to Player
        } else if (c) {
            c->takeDamage(amount);  // Apply damage to Card
        }
    });
}

Ability Ability::addMana(int amount, ManaType mana) {
    return Ability(abilityType::ADD_MANA, amount, [amount, mana](Player* p, Card*) {
        if (p) {
            QMap<ManaType, int> manaMap;
            manaMap[mana] = amount;
            p->addMana(&manaMap);  // Add mana to Player's pool
        }
    });
}
