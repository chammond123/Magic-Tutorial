#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include<QVector>
#include<QMap>
#include"card.h"
#include"player.h"
#include"gamestate.h"
#include"command.h"

/**
 * @brief The gamemanager class
 */
class gamemanager : public QObject
{
    Q_OBJECT
private:
    int mainWindowLeft;
    int mainWindowTop;
    int mainWindowRight;
    int mainWindowBottom;

public:
    GameState* state;
    gamemanager(QObject *parent);
    ~gamemanager();
    /**
     * @brief True if a player wants to display gmae tips during a game
     */
    bool displayGameTips;

    bool hasDisplayedWelcome = false;

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
    /**
     * @brief Displays a dialog box containing a tip for the phase
     * @param The tip message to show
     */
    void displayTip(QString tip, int xCoord = 1700, int yCoord = 950);
    /**
     * @brief Updates dialog when an action is made
     */
    void displayPhaseTip();
    /**
     * @brief Connected to the UI toggle tips check box, turns the tips on and off
     */
    void onToggleGameTips(bool toggled);

    void onReceiveMainWindowPos(int left, int right, int top, int bottom);
signals:
    void updateUI();
    void gameOver(bool hasWon);
    void startTargeting(Card* initialCard);
    void promptTargeting(Card* card);
    void requestMainWindowPos();
};

#endif // GAMEMANAGER_H
