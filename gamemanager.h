#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include<QVector>
#include"card.h"
#include"player.h"

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
        DeclairBlockers,
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
        Phase::CombatDamage,
        Phase::PostCombatMain,
        Phase::EndStep,
        Phase::Cleanup
    };
    QVector<Card> theStack;
    int turnCount = 0;
    Player activePlayer;
    Player passivePlayer;
public:
    gamemanager();
};

#endif // GAMEMANAGER_H
