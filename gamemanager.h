#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include<QVector>
#include<QMap>
#include"card.h"
#include"player.h"
#include"gamestate.h"
#include"command.h"

class gamemanager : public QObject
{
    Q_OBJECT
private:

public:
    GameState* state;
    gamemanager(QObject *parent);
    ~gamemanager();

public slots:
    /**
     * @brief Receives if a player wants to draw a card for turn
     * @param The player who wants to draw
     */
    void onCardDrawn(Player* player);
    /**
     * @brief Receives if a player wants to cast a card
     * @param The player who wants to play a card
     * @param The card they want to play
     */
    void onPlayCard(Player* player, Card* card, Card* target);
    /**
     * @brief Receives if a player wants to pass priority
     * @param The player that wants to pass
     */
    void onPassPriority(Player* player);
    /**
     * @brief Revies if a player wants to change the phase of the game
     * @param The player that wants to change phase
     */
    void onChangePhase(Player* player);
    /**
     * @brief Receives if a player wants to pass the turn
     * @param The player that wants to pass the turn
     */
    void onPassTurn(Player* player);
    /**
     * @brief Receives if a player wants to declare a creature as an attacker
     * @param The player who wants to declare an attacker
     * @param The card they want to declare as an attacker
     */
    void onCombatCardsReceived(QMap<Card*, QVector<Card*>> CombatCreatures);
    /**
     * @brief Receives if a player has lost the game
     */
    void onPlayerLost();

signals:
    void updateUI();
};

#endif // GAMEMANAGER_H
