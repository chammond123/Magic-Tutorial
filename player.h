#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include "card.h"
#include "zone.h"
#include "type.h"

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
    bool hasPlayedLand;
    bool hasntDrawnForTurn;
    bool holdingPriority;
    bool isActivePlayer;
    bool madeAction;
    QVector<Card *> Graveyard;
    QVector<Card *> Exile;
    QVector<Card *> Hand;
    QVector<Card *> Battlefield;

    // Health Methods
    int getHealth();

    // Zone Methods
    void moveCard(Card *card, QString sourceZone, QString targetZone);

    // Mana Methods
    void payMana(int manaCost, ManaType color);
    bool canPayMana(Card* card);

    // Turn Phases
    void onBlockRequested(Card *attacker, Card *defender = nullptr);
    void untapPhase();
    void upkeepPhase();

public slots:

    void playCard(int index, QString zone);
    void addMana(QMap<ManaType, int> *manaCosts);
    void useMana(QMap<ManaType, int> *manaCosts);
    void mill(int amount);
    void drawCard(int amount = 1);
    void takeDamage(int amount);
    void gainLife(int amount);
    void updateAllUI();
    void endTurn();

signals:

    void healthChanged(int newLife);
    void cardDrawn(Card *card);
    void cardPlayed(Card *card);
    void turnEnded();
    void playerLost();
    void handChanged();
    void battlefieldChanged();
    void graveyardChanged();
    void exileChanged();
    void libraryChanged();
    void manaPoolChanged(QMap<ManaType, int> *newMana);
    void requestDiscard(QString zone);
    void invalidAction(QString message);

private:
    int health;
    QMap<ManaType, int> manaPool;

    void loseGame();
    //   Card* findCardByID(int cardId, const QVector<Card*>& targetZone); TODO: Implement after Card Class
    Card *findCardInZone(int cardId, QString zoneRequested);
    QVector<Card*> findZone(QString zoneInput);
};

#endif // PLAYER_H
