#include "command.h"

Command::Command(GameState* state, Player* player, Card* card) :  state(state), player(player), card(card) {}
Command::~Command() {}

drawCommand::drawCommand(GameState* state, Player* player) : Command(state, player, nullptr){}
void drawCommand::execute(){
    player->drawCard();
}
bool drawCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (player->isActivePlayer && rules.canDraw && player->hasntDrawnForTurn){
        return true;
    }
    else{
        return false;
    }
}

playCardCommand::playCardCommand(GameState* state, Player* player, Card* card) : Command(state, player, card){}
void playCardCommand::execute(){
    player->madeAction = true;
    player->playCard(card);
}
bool playCardCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (player->isActivePlayer && rules.canPlaySorcery && card->isLand){
        return true;
    }
    else if (player->isActivePlayer && rules.canPlaySorcery && state->stackIsEmpty() && player->canPayMana(card)){
        return true;
    }
    else if (rules.canPlayInstant && player->canPayMana(card)){
        return true;
    }
    else{
        return false;
    }
}

passPriorityCommand::passPriorityCommand(GameState* state, Player* player) : Command(state, player, nullptr){}
void passPriorityCommand::execute(){
    if (!player->madeAction){
        // TODO:: Something with the stack
    }
    state->changePriority();
}
bool passPriorityCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (player->holdingPriority){
        return true;
    }
    else{
        return false;
    }
}

changePhaseCommand::changePhaseCommand(GameState* state, Player* player) : Command(state, player, nullptr){}
void changePhaseCommand::execute(){
    state->changePhase();
}
bool changePhaseCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (player->isActivePlayer && player->holdingPriority && state->stackIsEmpty()){
        return true;
    }
    else{
        return false;
    }
}

passTurnCommand::passTurnCommand(GameState* state, Player* player) : Command(state, player, nullptr){}
void passTurnCommand::execute(){
    state->changeActivePlayer();
}
bool passTurnCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (player->isActivePlayer && rules.canPassTurn){
        return true;
    }
    else{
        return false;
    }
}

declareAttackerCommand::declareAttackerCommand(GameState* state, Player* player, Card* card) : Command(state, player, card){}
void declareAttackerCommand::execute(){
    //player>declareAttacker(); // TODO: decide what this does
}
bool declareAttackerCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (player->isActivePlayer && rules.canDeclareAttack){
        return true;
    }
    else{
        return false;
    }
}

declareBlockerCommand::declareBlockerCommand(GameState* state, Player* player, Card* card) : Command(state, player, card){}
void declareBlockerCommand::execute(){
    //player->declareBlocker(); // TODO: this too
}
bool declareBlockerCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (!player->isActivePlayer && rules.canDeclareDefense){
        return true;
    }
    else{
        return false;
    }
}

tapCardCommand::tapCardCommand(GameState* state, Player* player, Card* card) : Command(state, player, card){}
void tapCardCommand::execute(){
    player->tapCard(card);
}
bool tapCardCommand::isValid(){
    PhaseRules rules = state->getPhaseRules();
    if (card->isLand){
        return true;
    }
    else{
        return false;
    }
}
