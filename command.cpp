#include "command.h"

Command::Command() {}

drawCommand::drawCommand(GameState* state, Player* player){}
drawCommand::execute(){
    player.drawCard();
}
drawCommand::isValid(){
    PhaseRules rules = state.getPhaseRules();
    if (player.isActivePlayer && rules.canDraw && player.hasntDrawnForTurn){
        return true;
    }
    else{
        return false;
    }
}

playCardCommand::playCardCommand(GameState* state, Player* player, Card* card){}
playCardCommand::execute(){
    player.playCard(card);
}
playCardCommand::isValid(){
    PhaseRules = state.GetPhaseRules();
    if (player.isActivePlayer && phase.canPlaySorcery && card.isLand){
        return true;
    }
    else if (player.isActivePlayer && phase.canPlaySorcery && state.stackIsEmpty() && player.canAfford(card)){
        return true;
    }
    else if (phase.canPlayInstant && player.canAfford(card)){
        return true;
    }
    else{
        return false;
    }
}

passPriorityCommand::passPriorityCommand(GameState* state, Player* player){}
passPriorityCommand::execute(){
    state.changePriority();
}
passPriorityCommand::isValid(){

}

passTurnCommand::passTurnCommand(GameState* state, Player* player){}

declareAttackerCommand::declareAttackerCommand(GameState* state, Player* player, Card* card){}

declareBlockerCommand::declareBlockerCommand(GameState* state, Player* player, Card* card){}
