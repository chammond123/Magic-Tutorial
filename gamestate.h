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
    struct PhaseRules {
        bool canPlayInstant;
        bool canPlaySorcery;
        bool canDraw;
        bool canDeclareAttack;
        bool canDeclareDefense;
        bool canPassTurn;
        bool canUntap;
    };
    Player player1;
    Player player2;

    Phase currentPhase;
    QVector<Ability> theStack;
    // Player* activePlayer;
    // Player* passivePlayer;
    // Player* priority;
    QVector<Card*> attackers;
    QVector<Card*> defenders;
    int turnCount;

public:
    GameState();

    /**
     * @brief Sets the current phase in the game state
     */
    void changePhase(Phase newPhase);

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

    /**
     * @brief Gets the ruels for the current phase
     * @return The Phase Rules for the current phase
     */
    PhaseRules getPaseRules();
};

#endif // GAMESTATE_H
