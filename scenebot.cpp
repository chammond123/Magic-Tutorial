#include "scenebot.h"
#include "command.h"

SceneBot::SceneBot(QStringList deckList, QObject *parent, GameState* state)
    : Player(deckList, parent), currentState(state){

    // Initialize the queue of commands

}

void SceneBot::executeCommand(){
    playCardCommand command = currentCommands.dequeue(); // TODO: Make
}
