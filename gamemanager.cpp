#include "gamemanager.h"

gamemanager::gamemanager(){
    state = new GameState();
}

// void gamemanager::mainGameLoop(Player* &player1, Player* &player2){
//     while (player1.alive && player2.alive){
//         for (const Phase& phase : phases){
//             switch (phase){
//                 case Phase::Untap:
//                     // Player untaps cards
//                     break;
//                 case Phase::Upkeep:
//                     // Player Upkeep trigger
//                     break;
//                 case Phase::Draw:
//                     // Player can draw a card
//                     break;
//                 case Phase::PreCombatMain:
//                     // Player can play a land, cast a card or activate an ability
//                     break;
//                 case Phase::BeginCombat:
//                     // Players may play instant speed cards or activate abilites
//                     break;
//                 case Phase::DeclareAttackers:
//                     // ActivePlayer selects attacking creatures
//                     break;
//                 case Phase::DeclareBlockers:
//                     // PassivePlayer selects defending creatures
//                     break;
//                 case Phase::CombatDamage:
//                     // Based on the delcared attackers and blockers, resolve the damage
//                     break;
//                 case Phase::PostCombatMain:
//                     // Players can play a land, cast a card or activate an ability
//                     break;
//                 case Phase::EndStep:
//                     // Player endstep trigger
//                     break;
//                 case Phase::Cleanup:
//                     // Player discards excess cards
//                     break;
//             }
//         }
//     }
// }

// UI Player Commands
gamemanager::onCardDrawn(Player* player){
    Command cmd = drawCommand(state, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onPlayCard(Player *player, Card *card){
    Command cmd = playCardCommand(state, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onPassPriority(Player* player){
    Command cmd = passPriorityCommand(state, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onChangePhase(Player *player){
    Command cmd = changePhaseCommand(state, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onDeclareAttacker(Player *player, Card *card){
    Command cmd = passPriorityCommand(state, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onDeclareAttacker(Player *player, Card *card){
    Command cmd = declareAttackerCommand(state, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

gamemanager::onPassTurn(Player *player){
    Command cmd = passTurnCommand(state, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

// Player object communication
