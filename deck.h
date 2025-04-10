#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <QObject>

class Deck : public QObject
{
    Q_OBJECT
public:
    explicit Deck(QString fileName, QObject *parent = nullptr);

    QString fileName;
    QVector<Card> cards;

signals:
};

#endif // DECK_H
