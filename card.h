#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QImage>
#include "ability.h"

class Card
{
public:
    Card();

private:
    QString description;
    QImage image;
    Ability ability;
};

#endif // CARD_H
