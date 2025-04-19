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

Ability Ability::counter() {
    return Ability( abilityType::COUNTER, 0, [](Player* p, Card* c){
            if (c) c->isCountered = true;
        });
}

Ability Ability::bypassSummonSickness() {
    return Ability( abilityType::BYPASS_SUMMONING_SICKNESS, 0, [](Player* p, Card* c){
        if (p) p->hasSummoningSickness = false;
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

Ability Ability::drawCards(int amount) {
    return Ability(abilityType::DRAW, amount, [amount](Player* p, Card*) {
        if (p) {
            p->drawCard(amount);  // Add mana to Player's pool
        }
    });
}
