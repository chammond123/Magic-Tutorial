#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "type.h"
#include <QWidget>
#include "deck.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

    Deck Library;
    QVector<Card*> Graveyard;
    QVector<Card*> Exile;
    QVector<Card*> Hand;
    QVector<Card*> Battlefield;

    // Health Methods
    int getHealth();

    // Zone Methods


    // Mana Methods
    void payMana(int manaCost, ManaColor color);

    // Turn Phases


public slots:

    void playCard(int index, QString zone);
    void moveCard(int CardIndex, QString sourceZone, QString targetZone);
    void addMana(QMap<ManaColor, int>* manaCosts);
    void useMana(QMap<ManaColor, int>* manaCosts);
    void mill(int amount);
    void drawCard(int amount = 1);
    void takeDamage(int amount);
    void gainLife(int amount);
    void updateAllUI();
    void endTurn();



signals:

    void healthChanged(int newLife);
    void cardDrawn(Card* card);
    void cardPlayed(Card* card);
    void turnEnded();
    void playerLost();
    void handChanged();
    void battlefieldChanged();
    void graveyardChanged();
    void exileChanged();
    void libraryChanged();
    void manaPoolChanged(QMap<ManaColor, int>* newMana);
    void requestDiscard(QString zone);
    void invalidAction(QString message);

private:
    int health;
    QMap<ManaColor, int> manaPool;

    void loseGame();
 //   Card* findCardByID(int cardId, const QVector<Card*>& targetZone); TODO: Implement after Card Class
//    Card* findCardInZone(int cardId, QString zoneRequested); TODO: Implement after Card Class

    bool canPayMana(int manaCost, ManaColor color);
};

#endif // PLAYER_H
