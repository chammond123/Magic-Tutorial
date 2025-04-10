#ifndef CARD_H
#define CARD_H

#include <QImage>
#include <QString>
#include <QVector>
#include "ability.h"
#include "type.h"
#include <algorithm>
#include <QMap>

class Card
{
public:
    Card();
    Card(const Card& other);

    void useAbility();
    bool canBePlayed(const QVector<ManaType> &availableMana) const;
    bool hasKeyword(PropertyType keyword) const;

public:
    QString name;
    QString description;
    QImage image;
    ManaType color;
    Ability ability = Ability();
    CardType type;
    int toughness;
    int power;
    QMap<ManaType, int> cost;
    QVector<PropertyType> keywords;
    bool isTapped;
    bool isPermanent;
    bool isLand;
};

#endif // CARD_H
