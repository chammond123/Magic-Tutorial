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

changePhaseCommand::changePhaseCommand(GameState* state) :
    Command(state){}
void changePhaseCommand::execute(){
    qDebug() << "Change Phase executed";
    state->changePhase();
}

declareCombatCommand::declareCombatCommand(GameState* state, QMap<Card*, QVector<Card*>> CombatCreatures) :
    Command(state), CombatCreatures(CombatCreatures){}
void declareCombatCommand::execute(){
    state->resolveCombatDamage(CombatCreatures);
}

tapCardCommand::tapCardCommand(GameState* state, Card* card) :
    Command(state), card(card){}
void tapCardCommand::execute(){
    Player* player = state->getPriorityPlayer();
    player->tapCard(card);
}
