#ifndef GAMESTATE_H
#define GAMESTATE_H

#include<QVector>
#include"player.h"
#include"ability.h"
#include"card.h"

class GameState
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
    Phase currentPhase;
    QVector<Ability> theStack;
    Player* activePlayer;
    Player* passivePlayer;
    Player* priority;
    QVector<Card*> attackers;
    QVector<Card*> defenders;
    int turnCount;

public:
    gamestate();

    /**
     * @brief Sets the current phase in the game state
     */
    void changePhase();

    /**
     * @brief Changes player priority
     */
    void changePriority();

    /**
     * @brief Changes active player, ie. passes turn
     */
    void changeActivePlayer();

    /**
     * @brief Based on declared attackers and defenders resolve damage
     */
    void resolveCombatDamage();
};

#endif // GAMESTATE_H
