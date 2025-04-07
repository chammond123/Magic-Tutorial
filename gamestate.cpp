#include "gamestate.h"

gamestate::gamestate() {}

void GameState::changePhase(Phase newPhase){
    currentPhase = newPhase;
}

void GameState::changePriority(){
    if(player1.holdingPriority){
        player2.holdingPriority = true;
        player1.holdingPriority = false;
    }
    else{
        player1.holdingPriority = true;
        player2.holdingPriority = false;
    }
}

void GameState::changeActivePlayer(){
    if(player1.activePlayer){
        player2.activePlayer = true;
        player1.activePlayer = false;
    }
    else{
        player1.activePlayer = true;
        player2.activePlayer = false;
    }
}

void GameState::resolveCombatDamate(){
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
        // Players may play instant speed cards or activate abilites
        break;
    case Phase::DeclareAttackers:
        // ActivePlayer selects attacking creatures
        break;
    case Phase::DeclareBlockers:
        // PassivePlayer selects defending creatures
        break;
    case Phase::CombatDamage:
        // Based on the delcared attackers and blockers, resolve the damage
        break;
    case Phase::PostCombatMain:
        // Players can play a land, cast a card or activate an ability
        break;
    case Phase::EndStep:
        // Player endstep trigger
        break;
    case Phase::Cleanup:
        // Player discards excess cards
        break;
    }
}
