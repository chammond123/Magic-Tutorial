#ifndef SCENEBOT_H
#define SCENEBOT_H

#include "player.h"
#include "gamestate.h"
class Command;

#include <QMap>
class SceneBot : public Player
{
public:
    SceneBot(QStringList deckList, QObject *parent, GameState* state);

    GameState* currentState;

    QMap<int, QQueue<Command*>> Scenes;

    QQueue<Command*> currentCommands;

    void executeCommand();
};

#endif // SCENEBOT_H
