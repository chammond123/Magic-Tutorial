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

private:
    void playCard(GameState* gameState);

    void declareAttackers(GameState* gameState);

    void declareBlockers(GameState* gameState);

    void endTurn(GameState* gameState);

    void passTurn(GameState* gameState);

    int calculateManaValue(Card* card);
};

#endif // BOT_H
