#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <map>
#include "card.h"
#include "zone.h"
#include "type.h"
#include "deck.h"

class Player
{
public:
    explicit Player(std::vector<std::string> deckList);

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

    std::map<ManaType, int> manaPool;
    std::map<ManaType, int> selectedMana;

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

    std::vector<Zone*> getZones();

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
    void moveCardString(Card *card, std::string sourceZone, std::string targetZone, bool OnTop);
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

    void playCard(Card *card);
    void addMana(std::map<ManaType, int> *manaCosts);
    void mill(int amount);
    void drawCard(int amount = 1);
    void takeDamage(int amount);
    void gainLife(int amount);
    void endTurn();

private:

    void loseGame();
    Zone* findCardZone(Card* card);
    // QVector<Card*> findZone(QString zoneInput);
};

#endif // PLAYER_H
