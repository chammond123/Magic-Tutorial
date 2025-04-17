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

    CardButton* currentSelectedCard = nullptr;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVector<CardButton*> activeCards;

    CardButton* currentCard;
    QVector<CardButton*> selectedCards;

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
    // void collectAttackers();
    // void collectBlockers();

    // void updateUI(Player* player);

    // void toggleButton();

    /**
     * @brief updateMagnifier
     * @param card
     */
    void updateMagnifier(Card* card);

signals:

    void sendCombatCards(QHash<Card*, QVector<Card*>>);



private:
    Ui::MainWindow *ui;
    CardAPIManager* apiManager;
    QString manaTypeToString(ManaType type);
    QString deckString;
    GameState* statePointer;

    void updateZone(QGridLayout* container, Zone* zone);
    void clearSelection();
};
#endif // MAINWINDOW_H
