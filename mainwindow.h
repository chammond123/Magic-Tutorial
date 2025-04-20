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


struct ZoneLayout {
    QGridLayout* hand;
    QGridLayout* battlefield;
    QLabel* graveyard;
    QLabel* exile;
    QLabel* red;
    QLabel* green;
    QLabel* blue;
    QLabel* white;
    QLabel* black;
    QLabel* health;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Player* userPlayer;
    Player* enemyPlayer;

    CardButton* currentSelectedCard = nullptr;

    QMap<ManaType, QList<CardButton*>> landGroups;

    MainWindow(gamemanager *game, QWidget *parent = nullptr);
    ~MainWindow();

    QVector<CardButton*> activeCards;

    QVector<Card*> selectedCards;
    QVector<CardButton*> selectedButtons;

    QMap<CardButton*, QVector<CardButton*>> buttonCombatants;
    QMap<Card*, QVector<Card*>> combatants;

    void showAllCards();
    void overlayCards();


public slots:

    /**
     * @brief cardMoveFromLibray creates a QPushButton
     * with the card pointer's data, and moves it to
     * the specified zone on the window
     * @param zone
     */
    // void cardMovedFromLibrary(Card*, QString zone);

    /**
     * @brief cardZoneChanged changes the placement
     * of a card on the screen assuming the Card is
     * already a QPushButton
     */
    // void cardZoneChanged(Card*, QString zone);

    void handleCardSelected(CardButton* clicked);
    void collectAttackers();
    void collectBlockers();

    void updateUI();
    void startTargeting();

    // void toggleButton();
    void attackPhase();

    /**
     * @brief updateMagnifier
     * @param card
     */
    void updateMagnifier(Card* card);

    void on_playCardButton_clicked();

    void showLandPopup(ManaType type);

signals:

    void sendCombatCards(QMap<Card*, QVector<Card*>> combatCards);

private:
    Ui::MainWindow *ui;
    CardAPIManager* apiManager;
    QString manaTypeToString(ManaType type);
    QString deckString;
    GameState* statePointer;

    void updateZone(QGridLayout* container, Zone* zone);
    void clearSelection();

    QMap<CardButton*, QVector<CardButton*>>::iterator targetIt;
    void extractCombatants(QMap<CardButton*, QVector<CardButton*>> packedCombatCard);
    ZoneLayout playerLayout;
    ZoneLayout enemyLayout;
    void handlePhase();

    bool isTargeting;

    bool isLand(Card* card);
    bool isCreature(Card* card);

    QString parseDescription(Card* card);

    // FOR TESTING PURPOSES
    Card card1;
    Card card2;
    Card card3;
    Card card4;
    Card card5;
    Card card6;
    Card card7;
    Card card8;
    Card card9;
    Card card10;
    void setupHand();
    bool createFlag = true;

};


#endif // MAINWINDOW_H
