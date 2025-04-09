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

};

#endif // INPUTHANDLER_H
