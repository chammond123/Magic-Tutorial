#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <QObject>

class Deck : public QObject
{
    Q_OBJECT

private:

    QVector<Card> cards;
public:
    /**
     * @brief Deck Constructor taking in a fileName of cards
     * @param fileName
     * @param parent
     */
    explicit Deck(QString fileName, QObject *parent = nullptr);

    QVector<Card> getCardObjects();

signals:
};

#endif // DECK_H
