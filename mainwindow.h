#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlabel.h"
#include "type.h"
#include "zone.h"
#include "card.h"
#include "player.h"
#include "cardapimanager.h"
#include "gamestate.h"
#include <QMainWindow>
#include <QPushButton>
#include "gamemanager.h"
#include "cardbutton.h"
#include <QtWidgets/qgridlayout.h>
#include <Box2D/Box2D.h>
#include <QGraphicsScene>

/**
 * @brief The ZoneLayout class
 * These are the zones in the ui
 */
struct ZoneLayout {
    QGridLayout* hand;
    QGridLayout* battlefield;
    QPushButton* graveyard;
    QString graveName;
    QPushButton* exile;
    QString exileName;
    QLabel* red;
    QLabel* green;
    QLabel* blue;
    QLabel* white;
    QLabel* black;
    QPushButton* redCount;
    QPushButton* greenCount;
    QPushButton* blueCount;
    QPushButton* whiteCount;
    QPushButton* blackCount;
    QLabel* health;
    QLabel* phaseLabel;
    QLabel* activePlayerLabel;
    QMap<ManaType, QList<CardButton*>>* landGroups;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event) override;

public:
    /**
     * @brief userPlayer
     * Player pointer to the user player
     */
    Player* userPlayer;

    /**
     * @brief enemyPlayer
     * Player pointer to the enemy player
     */
    Player* enemyPlayer;

    /**
     * @brief currentSelectedCard is the current card being select by the user player
     */
    CardButton* currentSelectedCard = nullptr;

    /**
     * @brief playerLandGroups is a map variable in which type of mana is the key and a vector of lands that associate
     * to that type of mana is the values which is owned by the user player
     */
    QMap<ManaType, QList<CardButton*>> playerLandGroups;

    /**
     * @brief enemyLandGroups playerLandGroups is a map variable in which type of mana is the key and a vector of lands that associate
     * to that type of mana is the values which is owned by the enemy player
     */
    QMap<ManaType, QList<CardButton*>> enemyLandGroups;

    /**
     * @brief MainWindow
     * @param game is a GameManager object
     * @param parent
     * The constructor of MainWindow
     */
    MainWindow(gamemanager *game, QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief activeCards is a list of current active cards
     */
    QList<CardButton*> activeCards;

    /**
     * @brief selectedCards is a vector of cards that being selected by the user player
     */
    QVector<Card*> selectedCards;

    /**
     * @brief selectedButtons is a vector of CardButton that being selected by the user player
     */
    QVector<CardButton*> selectedButtons;

    /**
     * @brief buttonCombatants is a map of attackers as the key and vector of blockers as the value
     */
    QMap<CardButton*, QVector<CardButton*>> buttonCombatants;

    /**
     * @brief combatants is the same thing with buttonCombatants but the key is Card pointer instead of CardButton pointer
     */
    QMap<Card*, QVector<Card*>> combatants;

    /**
     * @brief containerCards is a map that match the zone name to CardButton that is belong to that zone
     */
    QMap<QString, QList<CardButton*>> containerCards;

    /**
     * @brief overlayCards
     * this method uses to order attackers and blockers
     */
    void overlayCards();


public slots:

    /**
     * @brief goToMainMenu
     */
    void goToMainMenu();

    /**
     * @brief handleCardSelected updates which single card is being selected
     * @param clicked is the card that being clicked
     */
    void handleCardSelected(CardButton* clicked);

    void cardTapped(CardButton* button);

    /**
     * @brief getMainWindowPos
     */
    void getMainWindowPos();

    /**
     * @brief collectAttackers in the battle phase
     */
    void collectAttackers();

    /**
     * @brief collectBlockers in the battle phase
     */
    void collectBlockers();

    /**
     * @brief promptForMana is called when the player play a card that can be paid with different kind of mana
     * this method allows the user to choose different mana to pay for it
     * @return indicate if the player paid for the cost of the card
     */
    bool promptForMana(Card* card);


    /**
     * @brief updateUI redraw the ui when the state has changed
     */
    void updateUI();

    /**
     * @brief startTargeting a slot that choose a target for the source card
     * @param sourceCard card that need a target
     */
    void startTargeting(Card* sourceCard);

    /**
     * @brief updateMagnifier uses to update the magnifier of the card that being hovered
     * @param card that being hovered
     */
    void updateMagnifier(Card* card);

    /**
     * @brief onPlayCardButtonClicked handles the logic when play button being clicked
     */
    void onPlayCardButtonClicked();


    /**
     * @brief cardBeingTapped handles tapping the card
     * @param card that being tapped
     * @param tapped current state of the card
     */
    void cardBeingTapped();


    /**
     * @brief onGameEnded called when the one of the player win the game
     * @param playerWon a boolean value if the user player won
     */
    void onGameEnded(bool playerWon);

    void stopTargeting();

    /**
     * @brief Interprets the Bot's actions and implements blockers
     * @param blockers
     */
    void botDeclareCombatants(QMap<Card*, QList<Card*>> botCombatants);

    void botDeclareAttackers(QList<Card*> attackers);

    void displayBlockers(QList<Card*> blockers);

signals:

    /**
     * @brief returnToMainMenu
     */
    void returnToMainMenu();

    /**
     * @brief sendCombatCards is a signal that send combatCards to the backend
     * @param combatCards
     */
    void sendCombatCards(QMap<Card*, QVector<Card*>> combatCards);

    /**
     * @brief playCard a signal that connected to the back end inform about which card being played
     * @param card that being played
     * @param target of the card that being played
     */
    void playCard(Card* card, std::variant<Player*, Card*, std::nullptr_t> target);


    /**
     * @brief tapCard is a signal to the backend when a card is tapped
     * @param card that being tapped
     */
    void tapCard(Card* card);


    /**
     * @brief used to show the first welcome display tip
     */
    void displayGameTip();

    void updateMainWindowPos(int left, int right, int top, int bottom);

private:
    /**
     * @brief ui
     */
    Ui::MainWindow *ui;

    /**
     * @brief apiManager
     */
    CardAPIManager* apiManager;

    /**
     * @brief manaTypeToString converts ManaType to string
     * @param type that being converted
     * @return string of that type
     */
    QString manaTypeToString(ManaType type);

    /**
     * @brief phaseTypeToString converts Phase type to string
     * @param phase that being converted
     * @return string of that phase
     */
    QString phaseTypeToString(Phase phase);

    /**
     * @brief cardTypeToString convert CardType to string
     * @param type that being converted
     * @return string of that card type
     */
    QString cardTypeToString(CardType type);

    /**
     * @brief deckString
     */
    QString deckString;

    /**
     * @brief statePointer is the GameState object of the game
     */
    GameState* statePointer;

    //For ending scene
    /**
     * These variables are used to create the ending scene involving Box2D
     */
    QGraphicsScene* endScene = nullptr;
    QGraphicsView* endView = nullptr;
    b2World* endWorld = nullptr;
    QTimer* endTimer = nullptr;
    QVector<QPair<QGraphicsPixmapItem*, b2Body*>> endfallingCards;
    void updateEndFall();
    void updateEndExplosion();

    /**
     * @brief updateZone update the current zone on the ui
     * @param container of cards in the zone
     * @param zone that being updated
     * @param landGroups
     */
    void updateZone(QGridLayout* container, Zone* zone, QMap<ManaType, QList<CardButton *> > *landGroups, bool player);

    /**
     * @brief updateDeck
     * @param zone
     * @param title
     * @param deckButton
     */
    void updateDeck(Zone* zone, QString title, QPushButton *deckButton);

    /**
     * @brief clearSelection clears all the current selecting cards
     */
    void clearSelection();

    /**
     * @brief showCollection shows the collection of cards in that title
     * @param title
     */
    void showCollection(QString title);

    /**
     * @brief targetIt used to assigning attackers to the blockers
     */
    QMap<Card*, QVector<Card*>>::iterator targetIt;

    /**
     * @brief extractCombatants
     * @param packedCombatCard
     */
    void extractCombatants();

    /**
     * @brief playerLayout
     */
    ZoneLayout playerLayout;

    /**
     * @brief enemyLayout
     */
    ZoneLayout enemyLayout;

    /**
     * @brief handlePhase handles the change of phase
     */
    void handlePhase();

    /**
     * @brief targetSource
     */
    Card* targetSource;

    /**
     * @brief isTargeting
     */
    bool isTargeting;

    CardButton* createCardButton(Card* card, bool player);

    //For mana zone
    /**
     * Use to show and update mana of the player
     */
    void showLandPopup(ManaType type, ZoneLayout layout);
    void updateManaButton(ManaType type, ZoneLayout layout);
    bool hasWelcomed = false;

    void lotusHandling();


    // FOR TESTING PURPOSES
    void setupHand();

};


#endif // MAINWINDOW_H
