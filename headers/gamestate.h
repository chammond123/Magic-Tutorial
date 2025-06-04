#ifndef GAMESTATE_H
#define GAMESTATE_H

#include<QVector>
#include"player.h"
#include"card.h"
#include"phase.h"
#include"stackobject.h"

/**
 * @brief The GameState class
 */
class GameState
{
private:
    /**
     * @brief A vector holding an order set of phases
     */
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
    /**
     * @brief Index of the current phase of the game
     */
    int currentPhaseIndex = 0;
    /**
     * @brief Sets the valid cards in a players hand
     * @param The player whos hand to validate
     */
    void validateHand(Player* player);
    /**
     * @brief Sets the valid cards in a players battlefield
     * @param The player whos battlefield to validate
     */
    void validateBattlefield(Player* player);
    /**
     * @brief Validates the actions a player can take
     * @param The players whos actions to validate
     */
    void validatePlayerActions(Player* player);
    /**
     * @brief Holds how many turns have taken place
     */
    int turnCount = 0;

public:
    /**
     * @brief Holds the active stack in a magic round
     */
    QVector<StackObject> theStack;
    /**
     * @brief GameState
     */
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
    void resolveCombatDamage(QMap<Card*, QList<Card*>> CombatCreatures);

    /**
     * @brief Resolves the top card in the stack
     */
    void resolveStack();

    /**
     * @brief Adds a stack object to the stack
     * @param A stack object
     */
    void addToStack(StackObject stackObject);

    /**
     * @brief Gets the rules for the current phase
     * @return The Phase Rules for the current phase
     */
    PhaseRules getPhaseRules();

    /**
     * @brief Gets the player with current priority
     * @return The player with current priority
     */
    Player* getPriorityPlayer();

    /**
     * @brief Checks if both players have passed priority without making a game action.
     * @param The player making the check
     */
    bool bothPlayersPassPriority();

    /**
     * @brief Holds the current phase of the game
     */
    Phase currentPhase;

    /**
     * @brief Checks if the stack is empty
     * @return True if is empty, False otherwise
     */
    bool stackIsEmpty();

    /**
     * @brief Returns a QString representing the state
     * @return A QString containing the state
     */
    QString toString();

    /**
     * @brief A pointer to the second player object
     */
    Player* player1;

    /**
     * @brief A pointer to the second player object
     */
    Player* player2;

    /**
     * @brief Attackers declared by a player
     */
    QVector<Card*> attackers;
    /**
     * @brief Defenders declared by a player
     */
    QVector<Card*> defenders;

    /**
     * @brief Bool reporting if there is a bot set as player2
     */
    bool hasBotPlayer = true;

    /**
     * @brief Sets all the valid actions for a given state
     */
    void getValidActions();

};

#endif // GAMESTATE_H
