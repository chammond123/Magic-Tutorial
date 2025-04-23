#ifndef PHASE_H
#define PHASE_H

/**
 * @brief The Phase enum
 */
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

/**
 * @brief The PhaseRules class
 */
struct PhaseRules {
    bool canPlayInstant;
    bool canPlaySorcery;
    bool canDraw;
    bool canDeclareAttack;
    bool canDeclareDefense;
    bool canPassTurn;
    bool canUntap;
};
#endif // PHASE_H
