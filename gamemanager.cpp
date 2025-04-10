#include "gamemanager.h"

gamemanager::gamemanager(){
    state = new GameState();
}


// UI Player Commands
gamemanager::onCardDrawn(Player* player){
    Command cmd = drawCommand(state, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onPlayCard(Player *player, Card *card){
    Command cmd = playCardCommand(state, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onPassPriority(Player* player){
    Command cmd = passPriorityCommand(state, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onChangePhase(Player *player){
    Command cmd = changePhaseCommand(state, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onDeclareAttacker(Player *player, Card *card){
    Command cmd = passPriorityCommand(state, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onDeclareAttacker(Player *player, Card *card){
    Command cmd = declareAttackerCommand(state, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onPassTurn(Player *player){
    Command cmd = passTurnCommand(state, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

// Player object communication
