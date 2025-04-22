#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include "card.h"
#include "zone.h"
#include "type.h"
#include "deck.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QStringList deckList, QObject *parent = nullptr);

    /**
     * @brief deck object to contain instantiated cards
     */
    Deck deck;

    /**
     * @brief Library, Graveyard, Exile, Hand, and Battlefields are all
     * zones the player can interact with. These are pointers to
     * instantiated Card objects in the Deck.
     */
    Zone Library;
    Zone Graveyard;
    Zone Exile;
    Zone Hand;
    Zone Battlefield;

    int health;

    QMap<ManaType, int> manaPool;
    QMap<ManaType, int> selectedMana;

    // Used by UI to check which zones to update
    int playerID;

    // bool if player has played a land
    bool hasPlayedLand;

    // bool if player hasn't drawn
    bool hasntDrawnForTurn;

    // bool if player is holding priority
    bool holdingPriority;

    // bool if player is the active player
    bool isActivePlayer;

    bool madeAction;

    bool hasSummoningSickness;

    QVector<Zone*> getZones();

    // Health Methods
    int getHealth();

    // Variables for valid commands
    bool canPassPriority;
    bool canChangePhase;
    bool canDrawCard;

    // Zone Methods
    /**
     * @brief allows player to take a card and move if from one zone to the next
     * @param card selected
     * @param source where card is located
     * @param target where card is moving
     * @param ontop
     */
    void moveCardString(Card *card, QString sourceZone, QString targetZone, bool OnTop);
    void moveCardZone(Card *card, Zone& sourceZone, Zone& targetZone, bool OnTop);

    // Mana Methods

    /**
     * @brief goes through player mana pool and pays for the
     * @param manaCost
     */
    void useMana(Card* card);
    bool canPayMana(Card* card);

    // Turn Phases
    void onBlockRequested(Card *attacker, Card *defender = nullptr);
    void untap();
    void upkeepPhase();
    void cleanupPhase();
    void tapCard(Card* card);
    void emptyManaPool();
    void endStepPhase();
    void resolveCard(Card* card);

public slots:

    void playCard(Card *card);
    void addMana(QMap<ManaType, int> *manaCosts);
    void mill(int amount);
    void drawCard(int amount = 1);
    void takeDamage(int amount);
    void gainLife(int amount);
    void endTurn();

signals:

    void cardDrawn(Card *card);
    void cardPlayed(Card *card);
    void turnEnded();
    void playerLost();
    void handChanged();
    void requestDiscard(QString zone);
    void invalidAction(QString message);

private:

    void loseGame();
    Zone* findCardZone(Card* card);
    // QVector<Card*> findZone(QString zoneInput);
};

#endif // PLAYER_H
