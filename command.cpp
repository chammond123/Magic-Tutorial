#include "command.h"
#include "stackobject.h"
#include "player.h"
#include "gamestate.h"
#include "card.h"

Command::Command(GameState* state, Player* player) : state(state), player(player){}
Command::~Command() {}

drawCommand::drawCommand(GameState* state, Player* player) : Command(state, player){}
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

playCardCommand::playCardCommand(GameState* state, Player* player, Card* card, Card* target) :
    Command(state, player), card(card), target(target){}
void playCardCommand::execute(){
    player->madeAction = true;
    if (card->isLand){
        //player->moveCardZone(card, "hand", "battlefeild", false);
    }
    else{
        player->useMana(card);
        state->addToStack(StackObject{player, card, target});
    }
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

passPriorityCommand::passPriorityCommand(GameState* state, Player* player) :
    Command(state, player){}
void passPriorityCommand::execute(){
    if (!player->madeAction){
        state->resolveStack();
    }
    state->changePriority();
}
bool passPriorityCommand::isValid(){
    if (player->holdingPriority){
        return true;
    }
    else{
        return false;
    }
}

changePhaseCommand::changePhaseCommand(GameState* state, Player* player) :
    Command(state, player){}
void changePhaseCommand::execute(){
    state->changePhase();
}
bool changePhaseCommand::isValid(){
    if (player->isActivePlayer && player->holdingPriority && state->stackIsEmpty()){
        return true;
    }
    else{
        return false;
    }
}

passTurnCommand::passTurnCommand(GameState* state, Player* player) :
    Command(state, player){}
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

declareCombatCommand::declareCombatCommand(GameState* state, Player* player, QMap<Card*, QVector<Card*>> CombatCreatures) :
    Command(state, player), CombatCreatures(CombatCreatures){}
void declareCombatCommand::execute(){
    state->resolveCombatDamage(CombatCreatures);
}
bool declareCombatCommand::isValid(){
    return true; // TODO: figure out how to validate this
}

tapCardCommand::tapCardCommand(GameState* state, Player* player, Card* card) :
    Command(state, player), card(card){}
void tapCardCommand::execute(){
    player->tapCard(card);
}
bool tapCardCommand::isValid(){
    if (card->isLand){
        return true;
    }
    else{
        return false;
    }
}
