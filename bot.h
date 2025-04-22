#ifndef BOT_H
#define BOT_H

#include "player.h"
#include "gamestate.h"
#include "card.h"

class Bot : public Player {
public:
    Bot(QStringList deckList);

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
