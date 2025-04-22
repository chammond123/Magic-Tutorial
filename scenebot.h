#ifndef SCENEBOT_H
#define SCENEBOT_H

#include "player.h"
#include "gamestate.h"
#include "command.h"

#include <QQueue>
#include <QMap>
class SceneBot : public Player
{
public:
    SceneBot(QStringList deckList, QObject *parent, GameState* state);

    GameState* currentState;

    QMap<QString, QQueue<Command*>> scenes;

    QQueue<Command*> currentCommands;

    /**
     * @brief Executes the next command in the queue
     */
    void executeCommand();

    /**
     * @brief Selects the scene commands to execute
     * @param A string representing the scenes to load
     */
    void selectScene(QString scene);
};

#endif // SCENEBOT_H
