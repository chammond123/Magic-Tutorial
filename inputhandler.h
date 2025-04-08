#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "player.h"
#include "card.h"
#include "gamestate.h"
#include "command.h"

class InputHandler
{
private:
    GameState* gameState;
public:
    InputHandler(GameState* state);
public slots:
    void onCardDrawn(Player* player);
    void onPlayCard(Player* player, Card* card);
    void onPassPriority(Player* player);
    void onPassTurn(Player* player);
    void onDeclareAttacker(Player* player, Card* card);
    void onDeclareBlocker(Player* player, Card* card);
};

#endif // INPUTHANDLER_H
