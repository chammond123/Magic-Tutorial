#ifndef COMMAND_H
#define COMMAND_H

class Player;
class GameState;
#include "card.h"

#include <QVector>

class Command
{
public:
    Player* player;
    GameState* state;

    Command(GameState* state, Player* player);
    virtual ~Command() noexcept;

    virtual void execute() = 0;
    virtual bool isValid() = 0;
};

class drawCommand : public Command{
public:
    drawCommand(GameState* state, Player* player);

    virtual void execute();
    virtual bool isValid();
};

class playCardCommand : public Command{
public:
    Card* card;
    Card* target;
    playCardCommand(GameState* state, Player* player, Card* card, Card* target);

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
    QMap<Card*, QVector<Card*>> CombatCreatures;
    declareCombatCommand(GameState* state, Player* player, QMap<Card*, QVector<Card*>> CombatCreatures);

    virtual void execute();
    virtual bool isValid();
};

class tapCardCommand : public Command{
public:
    Card* card;
    tapCardCommand(GameState* state, Player* player, Card* card);

    virtual void execute();
    virtual bool isValid();
};

#endif // COMMAND_H
