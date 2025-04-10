#include "gamemanager.h"

gamemanager::gamemanager(){
    state = new GameState();
}


// UI Player Commands
void gamemanager::onCardDrawn(Player* player){
    drawCommand cmd = drawCommand(state, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

void gamemanager::onPlayCard(Player *player, Card *card){
    playCardCommand cmd = playCardCommand(state, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

void gamemanager::onPassPriority(Player* player){
    passPriorityCommand cmd = passPriorityCommand(state, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

void gamemanager::onChangePhase(Player *player){
    changePhaseCommand cmd = changePhaseCommand(state, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

void gamemanager::onDeclareAttacker(Player *player, Card *card){
    declareAttackerCommand cmd = declareAttackerCommand(state, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

void gamemanager::onDeclareBlocker(Player *player, Card *card){
    declareBlockerCommand cmd = declareBlockerCommand(state, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

void gamemanager::onPassTurn(Player *player){
    passTurnCommand cmd = passTurnCommand(state, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

void gamemanager::onPlayerLost(){
    return;
}
// Player object communication
