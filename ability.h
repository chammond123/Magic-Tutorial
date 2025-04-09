#ifndef ABILITY_H
#define ABILITY_H

#include <QString>

class Player;
class Card;

class Ability
{

private:
    // QString name;
    // QString stringEffect;
    std::function<void(Player* player, Card* card)> effect;


public:
    Ability(std::function<void(Player*, Card*)> eff);
    void activate(Player* player, Card* target);

    static Ability damageCreature(int amount);
    static Ability damagePlayer(int amount);
    static Ability buff(int amount);
    static Ability heal(int amount);
    static Ability addMana(int amount);
    static Ability destroy();

};

#endif // ABILITY_H
