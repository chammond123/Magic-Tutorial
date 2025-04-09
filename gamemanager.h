#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include<QVector>
#include"card.h"
#include"player.h"
#include"gamestate.h"
#include"phase.h"

class gamemanager
{
private:

    void mainGameLoop(Player player1, Player player2);

public:
    GameState* state;
    gamemanager();

public slots:
    void onCardDrawn(Player* player);
    void onPlayCard(Player* player, Card* card);
    void onPassPriority(Player* player);
    void onChangePhase(Player* player);
    void onPassTurn(Player* player);
    void onDeclareAttacker(Player* player, Card* card);
    void onDeclareBlocker(Player* player, Card* card);
};

#endif // GAMEMANAGER_H
