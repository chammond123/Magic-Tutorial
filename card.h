#ifndef CARD_H
#define CARD_H

#include <QImage>
#include <QString>
#include <QVector>
#include "ability.h"
#include "type.h"
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
    QVector<PropertyType> getKeywords() const;
    bool isPermanent() const;

    void useAbility();
    bool canBePlayed(const QVector<ManaType> &availableMana) const;
    bool hasKeyword(PropertyType keyword) const;
    Card *clone() const;

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
    QVector<PropertyType> keywords;
};

#endif // CARD_H
