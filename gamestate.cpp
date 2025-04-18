#include "gamestate.h"

GameState::GameState(){}

void GameState::changePhase(){
    if (currentPhaseIndex >= phases.size() - 1){
        currentPhaseIndex = 0;
    }
    else{
        currentPhaseIndex ++;
    }
    currentPhase = phases[currentPhaseIndex];
    if (currentPhase == Phase::Untap){
        player1->emptyManaPool();
        player2->emptyManaPool();
        player1->untap();
        player2->untap();
    }
    else if (currentPhase == Phase::Upkeep){
        player1->upkeepPhase();
        player2->upkeepPhase();
    }
    else if(currentPhase == Phase::PreCombatMain){
        player1->emptyManaPool();
        player2->emptyManaPool();
    }
    else if(currentPhase == Phase::BeginCombat){
        player1->emptyManaPool();
        player2->emptyManaPool();
    }
    else if(currentPhase == Phase::PostCombatMain){
        player1->emptyManaPool();
        player2->emptyManaPool();
    }
    // else if (currentPhase == Phase::EndStep){
    //     player1->emptyManaPool();
    //     player2->emptyManaPool();
    //     player1->endStepPhase();
    //     player2->endstepPhase();
    // }
    else if (currentPhase == Phase::Cleanup){
        player1->emptyManaPool();
        player2->emptyManaPool();
        player1->cleanupPhase();
        player2->cleanupPhase();
    }
}

void GameState::changePriority(){
    if(player1->holdingPriority){
        player1->madeAction = false;
        player2->holdingPriority = true;
        player1->holdingPriority = false;
    }
    else{
        player2->madeAction = false;
        player1->holdingPriority = true;
        player2->holdingPriority = false;
    }
}

void GameState::changeActivePlayer(){
    if (player1->isActivePlayer){
        player2->holdingPriority = true;
        player2->isActivePlayer = true;
        player1->isActivePlayer = false;
    }
    else{
        player1->holdingPriority = true;
        player1->isActivePlayer = true;
        player2->isActivePlayer = false;
    }
}

void GameState::resolveCombatDamage(QMap<Card*, QVector<Card*>> CombatCreatures){
    Player* attackingPlayer;
    Player* defendingPlayer;
    if (player1->isActivePlayer){
        attackingPlayer = player1;
        defendingPlayer = player2;
    }
    else{
        attackingPlayer = player2;
        defendingPlayer = player1;
    }
    for (Card* attacker : CombatCreatures.keys()){
        // Holds the attackers power, whenever damage is delt to a creature the excess is stored here
        int powerLeftToDeal = attacker->power;

        // If the attacker has no blockers, deal damage to the player equal to its toughness
        if (CombatCreatures[attacker].empty()){
            defendingPlayer->takeDamage(attacker->power);
        }
        // Otherwise deal damage to the blocking creatures with powerLeftToDeal
        else{
            for (Card* defender : CombatCreatures[attacker]){
                if (powerLeftToDeal > 0){
                    defender->takeDamage(powerLeftToDeal);
                    powerLeftToDeal -= defender->toughness;
                    attacker->takeDamage(defender->toughness);
                }
                // If the defending creature dies, send it to the graveyard
                if (defender->toughness <= 0){
                    defendingPlayer->moveCardString(defender, "battlefeild", "graveyard", true);
                }
            }
            // If the attacking creature dies, send it to the graveyard
            if (attacker->toughness <= 0){
                attackingPlayer->moveCardString(attacker, "battlefeild", "graveyard", true);
            }
        }
    }
}

void GameState::addToStack(StackObject stackObject)
{
    theStack.append(stackObject);
}

void GameState::resolveStack(){
    if (!theStack.empty()){
        StackObject stackObject = theStack.takeLast();

        if (stackObject.target == nullptr){
            stackObject.card->ability.use(stackObject.target);
        }

        if (stackObject.card->isPermanent){
            stackObject.player->moveCardString(stackObject.card, "hand", "battlefield", false);
        }
        else{
            stackObject.player->moveCardString(stackObject.card, "hand", "battlefield", true);
        }
    }
}

PhaseRules GameState::getPhaseRules(){
    switch(currentPhase){
    case Phase::Untap:
        return PhaseRules{false, false, false, false, false, false, true};
        break;
    case Phase::Upkeep:
        // Player Upkeep trigger
        return PhaseRules{false, false, false, false, false, false, false};
        break;
    case Phase::Draw:
        return PhaseRules{true, false, true, false, false, false, false};
        // Player can draw a card
        break;
    case Phase::PreCombatMain:
        return PhaseRules{true, true, true, false, false, false, false};
        // Player can play a land, cast a card or activate an ability
        break;
    case Phase::BeginCombat:
        return PhaseRules{true, false, false, false, false, false, false};
        // Players may play instant speed cards or activate abilites
        break;
    case Phase::DeclareAttackers:
        return PhaseRules{false, false, false, true, false, false, false};
        // ActivePlayer selects attacking creatures
        break;
    case Phase::DeclareBlockers:
        return PhaseRules{false, false, false, false, true, false, false};
        // PassivePlayer selects defending creatures
        break;
    case Phase::CombatDamage:
        return PhaseRules{false, false, false, false, false, false, false};
        // Based on the delcared attackers and blockers, resolve the damage
        break;
    case Phase::PostCombatMain:
        return PhaseRules{true, true, true, false, false, false, false};
        // Players can play a land, cast a card or activate an ability
        break;
    case Phase::EndStep:
        return PhaseRules{true, false, false, false, false, false, false};
        // Player endstep trigger
        break;
    case Phase::Cleanup:
        return PhaseRules{false, false, false, false, false, true, false};
        // Player discards excess cards
        break;
    }
}
bool GameState::stackIsEmpty(){
    return theStack.isEmpty();
}
QString GameState::toString(){

}
