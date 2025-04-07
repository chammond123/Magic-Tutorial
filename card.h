#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QImage>
#include "ability.h"
#include "type.h"
#include <QVector>

class Card
{
public:
    Card();

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
