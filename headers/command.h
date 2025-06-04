#ifndef COMMAND_H
#define COMMAND_H

class Player;
class GameState;
#include "card.h"

#include <QVector>

/**
 * @brief The Command class
 */
class Command
{
public:
    GameState* state;

    Command(GameState* state);
    virtual ~Command() noexcept;

    virtual void execute() = 0;
};

class playCardCommand : public Command{
public:
    Card* card;
    std::variant<Player*, Card*, std::nullptr_t> target;

    playCardCommand(GameState* state, Card* card, std::variant<Player*, Card*, std::nullptr_t> target);

    virtual void execute();
};

class passPriorityCommand : public Command{
public:
    passPriorityCommand(GameState* state);

    virtual void execute();
};

class changePhaseCommand : public Command{
public:
    changePhaseCommand(GameState* state);

    virtual void execute();
};

class declareCombatCommand : public Command{
public:
    QMap<Card*, QVector<Card*>> CombatCreatures;
    declareCombatCommand(GameState* state, QMap<Card*, QVector<Card*>> CombatCreatures);

    virtual void execute();
};

class tapCardCommand : public Command{
public:
    Card* card;
    tapCardCommand(GameState* state, Card* card);

    virtual void execute();
};

#endif // COMMAND_H
