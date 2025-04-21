#include "scenebot.h"
#include "command.h"

SceneBot::SceneBot(QStringList deckList, QObject *parent, GameState* state)
    : Player(deckList, parent), currentState(state){
    // Initialize a bunch of pass priority commands for basic testing
    for (int i = 0; i < 100; i++){
        QString stringTest = "passCommand";
        Player* test = this;
        Command* passCommand = new passPriorityCommand(currentState, test);
        scenes[stringTest].append(passCommand);
    }
}

void SceneBot::selectScene(QString scene){
    if (scenes.contains(scene)){
        currentCommands = scenes[scene];
    }
}

void SceneBot::executeCommand(){
    Command* command = currentCommands.dequeue();
    command->execute();
}
