#include "command.h"

Command::Command() {}

drawCommand::drawCommand(GameState* state, Player* player){}
void drawCommand::execute(){
    player.drawCard();
}
bool drawCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (player.isActivePlayer && rules.canDraw && player.hasntDrawnForTurn){
        return true;
    }
    else{
        return false;
    }
}

playCardCommand::playCardCommand(GameState* state, Player* player, Card* card){}
void playCardCommand::execute(){
    player->playCard(card);
}
bool playCardCommand::isValid(){
    PhaseRules rules = state.GetPhaseRules();
    if (player->isActivePlayer && rules.canPlaySorcery && card->isLand()){
        return true;
    }
    else if (player->isActivePlayer && rules.canPlaySorcery && state.stackIsEmpty() && player->canPlayCard(card)){
        return true;
    }
    else if (rules.canPlayInstant && player->canAfford(card)){
        return true;
    }
    else{
        return false;
    }
}

passPriorityCommand::passPriorityCommand(GameState* state, Player* player){}
void passPriorityCommand::execute(){
    state->changePriority();
}
bool passPriorityCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (player->holdsPriority){
        return true;
    }
    else{
        return false;
    }
}

passTurnCommand::passTurnCommand(GameState* state, Player* player){}
void passTurnCommand::execute(){
    state->changeActivePlayer();
}
bool passTurnCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (player->isActivePlayer && rules.canPass){
        return true;
    }
    else{
        return false;
    }
}

declareAttackerCommand::declareAttackerCommand(GameState* state, Player* player, Card* card){}
void delareAttackerCommand::execute(){
    player.declareAttacker(); // TODO: decide what this does
}
bool declareAttackerCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (player->isActivePlayer && rules.canDeclareAttacker){
        return true;
    }
    else{
        return false;
    }
}

declareBlockerCommand::declareBlockerCommand(GameState* state, Player* player, Card* card){}
void declareBlockerCommand::execute(){
    player.declareBlocker(); // TODO: this too
}
bool declareBlockerCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (!player->isActivePlayer && rules.canDeclareBlocker){
        return true;
    }
    else{
        return false;
    }
}
