#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QImage>

class Card
{
public:
    Card();

private:
    QString description;
    QImage image;
};

#endif // CARD_H
