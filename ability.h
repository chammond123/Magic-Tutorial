#ifndef ABILITY_H
#define ABILITY_H

#include <QString>

class Player;
class Card;

class Ability
{
private:
    QString name;
    QString stringEffect;
    std::function<void(Player *player, Card *card)> effect;

public:
    Ability(QString name, QString description, std::function<void(Player *, Card *)> eff);
    void activate(Player *player, Card *target);
};

#endif // ABILITY_H
