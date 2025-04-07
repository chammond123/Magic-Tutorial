#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QImage>
#include "ability.h"
#include "type.h"
#include <QVector>
#include "mana.h"

class Card
{
public:
    Card();

private:
    QString name;
    QString description;
    QImage image;
    Mana color;
    Ability ability;
    Type type;
    int toughness;
    int power;
    QVector<Mana> cost;
};

#endif // CARD_H
