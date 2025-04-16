#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    CardButton* currentSelectedCard = nullptr;

    MainWindow(gamemanager *game, QWidget *parent = nullptr);
    ~MainWindow();

    QVector<CardButton*> activeCards;

    CardButton* currentCard;

    QVector<Card*> selectedCards;
    QVector<CardButton*> selectedButtons;

    QMap<CardButton*, QVector<CardButton*>> buttonCombatants;
    QMap<Card*, QVector<Card*>> combatants;

    void showAllCards(QWidget Zone);

public slots:

    /**
     * @brief cardMoveFromLibray creates a QPushButton
     * with the card pointer's data, and moves it to
     * the specified zone on the window
     * @param zone
     */
    void cardMovedFromLibray(Card*, QString zone);

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

    // void toggleButton();
    void attackPhase();

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
};
#endif // MAINWINDOW_H
