#include "ability.h"

Ability::Ability(QString name, QString description, std::function<void(Player *, Card *)> eff)
{
    this->name = name;
    stringEffect = description;
    effect = eff;
}

// Activate method
void Ability::activate(Player *player, Card *target)
{
    if (effect) {
        effect(player, target);
    }
}
