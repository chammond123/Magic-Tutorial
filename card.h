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
    Card& operator=(const Card& other);
    //For testing
    Card(QString name);

    bool canBePlayed(const QVector<ManaType> &availableMana) const;
    bool hasKeyword(PropertyType keyword) const;
    //take damage
    void takeDamage(int damage);
    //gain health
    void gainHealth(int health);
    //reset health
    void cleanupCard();

public:
    int currHealth = toughness;
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
