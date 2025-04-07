#include "gamemanager.h"

gamemanager::gamemanager() {}

void gamemanager::mainGameLoop(Player* &player1, Player* &player2){
    activePlayer = player1;
    passivePlayer = player2;
    while (player1.alive && player2.alive){
        for (const Phase& phase : phases){
            switch (phase){
                case Phase::Untap:
                    // Player untaps cards
                    break;
                case Phase::Upkeep:
                    // Player Upkeep trigger
                    break;
                case Phase::Draw:
                    // Player can draw a card
                    break;
                case Phase::PreCombatMain:
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
                changePhase = false;
                while (!changePhase){

                }
        }
    }
}
