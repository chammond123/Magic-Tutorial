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
     * @brief Receives if a player wants to cast a card
     * @param The player who wants to play a card
     * @param The card they want to play
     */
    void onPlayCard(Card* card, std::variant<Player*, Card*, std::nullptr_t> target);
    /**
     * @brief Receives if a player wants to pass priority
     * @param The player that wants to pass
     */
    void onPassPriority();
    /**
     * @brief Revies if a player wants to change the phase of the game
     * @param The player that wants to change phase
     */
    void onChangePhase();
    /**
     * @brief Receives if a player wants to declare a creature as an attacker
     * @param The player who wants to declare an attacker
     * @param The card they want to declare as an attacker
     */
    void onCombatCardsReceived(QMap<Card*, QVector<Card*>> CombatCreatures);
    /**
     * @brief Receives if a plyer has taped a card
     */
    void onTapCard(Card* card);

signals:
    void updateUI();
    void gameOver(bool hasWon);
    void startTargeting(Card* initialCard);
};

#endif // GAMEMANAGER_H
