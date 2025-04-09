#ifndef GAMESTATE_H
#define GAMESTATE_H

#include<QVector>
#include"player.h"
#include"ability.h"
#include"card.h"
#include"phase.h"

class GameState
{
private:
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
    int currentPhaseIndex = 0;

    Player* player1;
    Player* player2;

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

    /**
     * @brief Gets the ruels for the current phase
     * @return The Phase Rules for the current phase
     */
    PhaseRules getPaseRules();

    /**
     * @brief Checks if both players have passed priority without making a game action.
     * @param The player making the check
     */
    bool bothPlayersPassPriority();

};

#endif // GAMESTATE_H
