#ifndef COMMAND_H
#define COMMAND_H

#include "gamestate.h"
#include "player.h"
#include "card.h"

class Command
{
public:
    Command(GameState* state, Player* player, Card* card, QVector* targets);
    virtual ~Command();

    virtual void execute() = 0;
    virtual bool isValid() = 0;

    Player* player;
    GameState* state;
    Card* card;
    QVector* targets;
};

class drawCommand : public Command{
public:
    drawCommand(GameState* state, Player* player);

    virtual void execute();
    virtual bool isValid();
};

class playCardCommand : public Command{
public:
    playCardCommand(GameState* state, Player* player, Card* card);

    virtual void execute();
    virtual bool isValid();
};

class passPriorityCommand : public Command{
public:
    passPriorityCommand(GameState* state, Player* player);

    virtual void execute();
    virtual bool isValid();
};

class changePhaseCommand : public Command{
public:
    changePhaseCommand(GameState* state, Player* player);

    virtual void execute();
    virtual bool isValid();
};

class passTurnCommand : public Command{
public:
    passTurnCommand(GameState* state, Player* player);

    virtual void execute();
    virtual bool isValid();
};

class declareCombatCommand : public Command{
public:
    declareCombatCommand(GameState* state, Player* player, Card* card);

    virtual void execute();
    virtual bool isValid();
};

class tapCardCommand : public Command{
public:
    tapCardCommand(GameState* state, Player* player, Card* card);

    virtual void execute();
    virtual bool isValid();
};

#endif // COMMAND_H
