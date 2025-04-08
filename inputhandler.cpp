#include "inputhandler.h"

InputHandler::InputHandler(GameState* state) {
    gameState = state;
}

InputHandler::onCardDrawn(Player* player){
    Command cmd = drawCommand(gameState, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

InputHandler::onPlayCard(Player *player, Card *card){
    Command cmd = playCardCommand(gameState, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

InputHandler::onPassPriority(Player* player){
    Command cmd = passPriorityCommand(gameState, player);
    if (cmd.isValid()){
        cmd.execute();
    }
}

InputHandler::onDeclareAttacker(Player *player, Card *card){
    Command cmd = passPriorityCommand(gameState, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

InputHandler::onDeclareAttacker(Player *player, Card *card){
    Command cmd = declareAttackerCommand(gameState, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}

InputHandler::onPassTurn(Player *player){
    Command cmd = passTurnCommand(gameState, player, card);
    if (cmd.isValid()){
        cmd.execute();
    }
}
