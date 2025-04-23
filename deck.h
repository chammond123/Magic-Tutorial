#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <QObject>

/**
 * @brief The Deck class
 */
class Deck : public QObject
{
    Q_OBJECT


private:

    QVector<Card*> cards;
public:
    /**
     * @brief Deck Constructor that instantiates all cards
     * @param deckList
     * @param parent
     */
    explicit Deck(QStringList deckList, QObject *parent = nullptr);

    QVector<Card*> getCardObjects();

    ~Deck();

signals:
};

#endif // DECK_H
