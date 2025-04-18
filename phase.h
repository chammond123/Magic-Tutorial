#ifndef PHASE_H
#define PHASE_H

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
#endif // PHASE_H
