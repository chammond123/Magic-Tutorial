#ifndef ABILITY_H
#define ABILITY_H

#include <QString>
#include <functional>
#include "type.h"

class Player;
class Card;

/**
 * @brief The Ability class represents the ability of the card
 */
class Ability
{
private:
    /**
     * @brief effect is a functor that execute the ability of the card
     */
    std::function<void(Player* player, Card* card)> effect;

    /**
     * @brief type of the ability
     */
    abilityType type;

    /**
     * @brief amount is the quantifier for the ability effect
     */
    int amount;

public:
    // Constructor to initialize ability with type, amount, and effect
    Ability(abilityType type = abilityType(), int amount = 0, std::function<void(Player*, Card*)> eff = nullptr);

    // Use ability on a target (either Player or Card)
    void use(Player* player);
    void use(Card* card);

    // Static functions for creating specific abilities
    static Ability damageTarget(int amount);
    static Ability heal(int amount);
    static Ability addMana(int amount, ManaType mana);
    static Ability destroy();
    static Ability drawCards(int amount);
    static Ability counter();
    static Ability bypassSummonSickness();
};

#endif // ABILITY_H
