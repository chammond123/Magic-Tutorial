#ifndef BOT_H
#define BOT_H

#include "player.h"
#include "gamestate.h"
#include "card.h"

/**
 * @brief The Bot class acts as an opponent to the user player
 * This bot can really play as a player
 */
class Bot : public Player {

    Q_OBJECT
public:
    /**
     * Constructor of the bot that take in a deckList
     * @param deckList
     */
    Bot(QStringList deckList);

    /**
     * @brief takeTurn
     * @param gameState is the currentGameState
     */
    void takeTurn(GameState* gameState);

public slots:

signals:
    /**
     * @brief Declares blockers to be used in the UI
     * @param blockers against a card
     */
    void UiDeclareCombatants(QMap<Card*, QList<Card*>> botCombatants);

    void UiDeclareAttackers(QList<Card*> attackers);

    void showBlockers(QList<Card*> blockers);

private:
    /**
     * @brief playCard
     * @param gameState
     */
    void playCard(GameState* gameState);

    /**
     * @brief declareAttackers
     * @param gameState
     */
    void declareAttackers(GameState* gameState);

    /**
     * @brief declareBlockers
     * @param gameState
     */
    void declareBlockers(GameState* gameState);

    /**
     * @brief endTurn
     * @param gameState
     */
    void endTurn(GameState* gameState);

    /**
     * @brief passPriority
     * @param gameState
     */
    void passPriority(GameState* gameState);

    /**
     * @brief changePhase
     * @param gameState
     */
    void changePhase(GameState* gameState);

    /**
     * @brief calculateManaValue
     * @param card
     * @return
     */
    int calculateManaValue(Card* card);
};

#endif // BOT_H
