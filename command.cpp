#include "command.h"
#include "stackobject.h"
#include "player.h"
#include "gamestate.h"
#include "card.h"
#include "bot.h"
#include <type_traits>

using namespace std;

Command::Command(GameState* state) : state(state){}
Command::~Command() {}

playCardCommand::playCardCommand(GameState* state, Card* card, std::variant<Player*, Card*, std::nullptr_t> target) :
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
        if (card->name.toLower() == "divination" or card->name.toLower() == "fervor"){
            target = state->getPriorityPlayer();
        }
        state->addToStack(StackObject{state->getPriorityPlayer(), card, target});
        card->isOnStack = true;
    }
}

passPriorityCommand::passPriorityCommand(GameState* state) :
    Command(state){}
void passPriorityCommand::execute(){
    if (!state->player1->madeAction && !state->player2->madeAction){
        state->resolveStack();
    }

    state->changePriority();

    if (state->player2->holdingPriority){
        Bot* botPlayer = static_cast<Bot*>(state->player2);
        botPlayer->takeTurn(state);
    }
}

changePhaseCommand::changePhaseCommand(GameState* state) :
    Command(state){}
void changePhaseCommand::execute(){
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
    if (card->type == CardType::LAND){
        card->ability.use(player);
        player->tapCard(card);
    }
    else if (card->type == CardType::ARTIFACT){
        card->ability.use(player);
        player->moveCardString(card, "battlefield", "graveyard", true);
    }
}
