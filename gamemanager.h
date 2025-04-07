#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include<QVector>
#include"card.h"
#include"player.h"
#include"gamestate.h"

class gamemanager
{
private:
    enum class Phase {
        Untap,
        Upkeep,
        Draw,
        PreCombatMain,
        BeginCombat,
        DeclareAttackers,
        DeclareBlockers,
        CombatDamage,
        PostCombatMain,
        EndStep,
        Cleanup
    };
    QVector<Phase> phases = {
        Phase::Untap,
        Phase::Upkeep,
        Phase::Draw,
        Phase::PreCombatMain,
        Phase::BeginCombat,
        Phase::DeclareAttackers,
        Phase::DeclareBlockers,
        Phase::CombatDamage,
        Phase::PostCombatMain,
        Phase::EndStep,
        Phase::Cleanup
    };
    GameState* state;
    void mainGameLoop(Player player1, Player player2);

public:
    gamemanager();

public slots:

};



#endif // GAMEMANAGER_H
