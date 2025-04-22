#include "command.h"
#include "stackobject.h"
#include "player.h"
#include "gamestate.h"
#include "card.h"

Command::Command(GameState* state) : state(state){}
Command::~Command() {}

drawCommand::drawCommand(GameState* state) : Command(state){}
void drawCommand::execute(){
    Player* player = state->getPriorityPlayer();
    player->drawCard();
}
bool drawCommand::isValid(){
    Player* player = state->getPriorityPlayer();
    PhaseRules rules = state->getPhaseRules();
    if (player->isActivePlayer && rules.canDraw && player->hasntDrawnForTurn){
        return true;
    }
    else{
        return false;
    }
}

playCardCommand::playCardCommand(GameState* state, Card* card, Card* target) :
    Command(state), card(card), target(target){}
void playCardCommand::execute(){
    Player* player = state->getPriorityPlayer();
    state->getPriorityPlayer()->madeAction = true;
    if (card->isLand){
        //player->moveCardZone(card, "hand", "battlefeild", false);
    }
    else{
        player->useMana(card);
        state->addToStack(StackObject{state->getPriorityPlayer(), card, target});
    }
}
bool playCardCommand::isValid(){
    Player* player = state->getPriorityPlayer();
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

passPriorityCommand::passPriorityCommand(GameState* state) :
    Command(state){}
void passPriorityCommand::execute(){
    qDebug() << "Command executed";
    Player* player = state->getPriorityPlayer();
    if (!player->madeAction){
        state->resolveStack();
    }
    // if (state->hasBotPlayer){
    //     state->player2->
    // }
    state->changePriority();
}
bool passPriorityCommand::isValid(){
    Player* player = state->getPriorityPlayer();
    if (player->holdingPriority){
        return true;
    }
    else{
        return false;
    }
}

changePhaseCommand::changePhaseCommand(GameState* state) :
    Command(state){}
void changePhaseCommand::execute(){
    qDebug() << "Change Phase executed";
    state->changePhase();
}
bool changePhaseCommand::isValid(){
    Player* player = state->getPriorityPlayer();
    if (player->isActivePlayer && player->holdingPriority && state->stackIsEmpty()){
        return true;
    }
    else{
        return false;
    }
}

declareCombatCommand::declareCombatCommand(GameState* state, QMap<Card*, QVector<Card*>> CombatCreatures) :
    Command(state), CombatCreatures(CombatCreatures){}
void declareCombatCommand::execute(){
    state->resolveCombatDamage(CombatCreatures);
}
bool declareCombatCommand::isValid(){
    return true; // TODO: figure out how to validate this
}

tapCardCommand::tapCardCommand(GameState* state, Card* card) :
    Command(state), card(card){}
void tapCardCommand::execute(){
    Player* player = state->getPriorityPlayer();
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
