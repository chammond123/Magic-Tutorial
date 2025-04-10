#include "gamestate.h"

gamestate::gamestate() {}

void GameState::changePhase(){
    if (currentPhaseIndex >= phases.size() - 1){
        currentPhaseIndex = 0;
    }
    else{
        currentPhaseIndex ++;
    }
    currentPhase = phases[currentPhaseIndex];
    if (currentPhase == Phase::Untap){
        player1->untap();
        player2->untap();
    }
    else if (currentPhase == Phase::Upkeep){
        player1->upkeepTrigger();
        player2->upkeepTrigger();
    }
    else if (currentPhase == Phase::EndStep){
        player1->endStepTrigger();
        player2->endstepTrigger();
    }
    else if (currentPhase == Phase::Cleanup){
        player1->cleanup();
        player2->cleanup();
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
        player1->activePlayer = false;
    }
    else{
        player1->holdingPriority = true;
        player1->isActivePlayer = true;
        player2->isActivePlayer = false;
    }
}

void GameState::resolveCombatDamage(){
    return;
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
