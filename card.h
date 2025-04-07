#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QImage>
#include "ability.h"
#include "type.h"
#include <QVector>
#include <algorithm>

class Card
{
public:
    Card();

    /*
     * Collection of getters for all instance variables
     */
    QString getName() const;
    QString getDescription() const;
    QImage getImage() const;
    ManaType getColor() const;
    Ability getAbility() const;
    CardType getType() const;
    int getToughness() const;
    int getPower() const;
    QVector<ManaType> getCost() const;
    bool isPermanent() const;

    void useAbility();
    bool canBePlayed(const QVector<ManaType> &availableMana) const;
    Card* clone() const;


private:
    QString name;
    QString description;
    QImage image;
    ManaType color;
    Ability ability;
    CardType type;
    int toughness;
    int power;
    QVector<ManaType> cost;
};

#endif // CARD_H
