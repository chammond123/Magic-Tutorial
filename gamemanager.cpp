// #include "gamemanager.h"

// gamemanager::gamemanager(){
//     state = new GameState();
// }


// // UI Player Commands
// void gamemanager::onCardDrawn(Player* player){
//     drawCommand cmd = drawCommand(state, player);
//     if (cmd.isValid()){
//         cmd.execute();
//         // emit updateUI();
//     }
// }

// void gamemanager::onPlayCard(Player *player, Card *card){
//     playCardCommand cmd = playCardCommand(state, player, card);
//     if (cmd.isValid()){
//         cmd.execute();
//         // emit updateUI();
//     }
// }

// void gamemanager::onPassPriority(Player* player){
//     passPriorityCommand cmd = passPriorityCommand(state, player);
//     if (cmd.isValid()){
//         cmd.execute();
//         // emit updateUI();
//     }
// }

// void gamemanager::onChangePhase(Player *player){
//     changePhaseCommand cmd = changePhaseCommand(state, player);
//     if (cmd.isValid()){
//         cmd.execute();
//         // emit updateUI();
//         // emit updateUIPhase();
//     }
// }

// void gamemanager::onCombatCardsReceived(Player *player, Card *card){
//     declareCombatCommand cmd = declareCombatCommand(state, player, card);
//     if (cmd.isValid()){
//         cmd.execute();
//         // emit updateUI();
//     }
// }

// void gamemanager::onPassTurn(Player *player){
//     passTurnCommand cmd = passTurnCommand(state, player);
//     if (cmd.isValid()){
//         cmd.execute();
//         // emit updateUI();
//     }
// }

// void gamemanager::onPlayerLost(){
//     return;
// }
// // Player object communication
