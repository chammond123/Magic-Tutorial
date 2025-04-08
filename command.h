#ifndef COMMAND_H
#define COMMAND_H

#include "gamestate.h"
#include "player.h"
#include "card.h"

class Command
{
public:
    virtual ~Command();

    virtual void execute() = 0;
    virtual bool isValid() = 0;
};

class drawCommand : public Command{
public:
    drawCommand(GameState* state, Player* player);

    virtual void execute();
    virtual void isValid();
};

class playCardCommand : public Command{
    playCardCommand(GameState* state, Player* player, Card* card);

    virtual void execute();
    virtual void isValid();
};

class passPriorityCommand : public Command{
    passPriorityCommand(GameState* state, Player* player);

    virtual void execute();
    virtual void isValid();
};

class passTurnCommand : public Command{
    passTurnCommand(GameState* state, Player* player);

    virtual void execute();
    virtual void isValid();
};

class declareAttackerCommand : public Command{
    declareAttackerCommand(GameState* state, Player* player, Card* card);

    virtual void execute();
    virtual void isValid();
};

class declareBlockerCommand : public Command{
    declareBlockerCommand(GameState* state, Player* player, Card* card);

    virtual void execute();
    virtual void isValid();
};

#endif // COMMAND_H
