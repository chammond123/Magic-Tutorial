#include "command.h"
#include "stackobject.h"
#include "player.h"
#include "gamestate.h"
#include "card.h"
#include "bot.h"
#include <type_traits>

Command::Command(GameState* state) : state(state){}
Command::~Command() {}

playCardCommand::playCardCommand(GameState* state, Card* card, Card* target) :
    Command(state), card(card), target(target){}
void playCardCommand::execute(){
    Player* player = state->getPriorityPlayer();
    state->getPriorityPlayer()->madeAction = true;
    if (card->isLand){
        player->moveCardString(card, "hand", "battlefield", false);
        player->hasPlayedLand = true;
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

    state->changePriority();

    Player* player = state->getPriorityPlayer();
    if (!player->madeAction){
        state->resolveStack();
    }
    if (state->player2->holdingPriority){
        Bot* botPlayer = static_cast<Bot*>(player);
        botPlayer->takeTurn(state);
    }
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
