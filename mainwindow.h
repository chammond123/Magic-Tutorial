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

public:
    Player* userPlayer;
    Player* enemyPlayer;

    CardButton* currentSelectedCard = nullptr;

    QMap<ManaType, QList<CardButton*>> playerLandGroups;
    QMap<ManaType, QList<CardButton*>> enemyLandGroups;

    MainWindow(gamemanager *game, QWidget *parent = nullptr);
    ~MainWindow();

    QList<CardButton*> activeCards;

    QVector<Card*> selectedCards;
    QVector<CardButton*> selectedButtons;

    QMap<CardButton*, QVector<CardButton*>> buttonCombatants;
    QMap<Card*, QVector<Card*>> combatants;

    QMap<QString, QList<CardButton*>> containerCards;

    void overlayCards();


public slots:


    void handleCardSelected(CardButton* clicked);
    void collectAttackers();
    void collectBlockers();

    bool promptForMana(Card* card);

    void updateUI();
    void startTargeting(Card* sourceCard);

    /**
     * @brief updateMagnifier
     * @param card
     */
    void updateMagnifier(Card* card);

    void onPlayCardButtonClicked();

    void cardBeingTapped();

    void onGameEnded(bool playerWon);

    void stopTargeting();

signals:

    void sendCombatCards(QMap<Card*, QVector<Card*>> combatCards);

    void playCard(Card* card, std::variant<Player*, Card*, std::nullptr_t> target);

    void tapCard(Card* card);

private:

    Ui::MainWindow *ui;
    CardAPIManager* apiManager;
    QString manaTypeToString(ManaType type);
    QString phaseTypeToString(Phase phase);
    QString cardTypeToString(CardType type);
    QString deckString;
    GameState* statePointer;

    //For ending scene
    QGraphicsScene* endScene = nullptr;
    QGraphicsView* endView = nullptr;
    b2World* endWorld = nullptr;
    QTimer* endTimer = nullptr;
    QVector<QPair<QGraphicsPixmapItem*, b2Body*>> endfallingCards;

    void updateEndFall();
    void updateEndExplosion();


    void updateZone(QGridLayout* container, Zone* zone, QMap<ManaType, QList<CardButton *> > *landGroups);
    void updateDeck(Zone* zone, QString title, QPushButton *deckButton);
    void clearSelection();
    void showCollection(QString title);

    QMap<CardButton*, QVector<CardButton*>>::iterator targetIt;
    void extractCombatants(QMap<CardButton*, QVector<CardButton*>> packedCombatCard);
    ZoneLayout playerLayout;
    ZoneLayout enemyLayout;
    void handlePhase();

    Card* targetSource;

    bool isTargeting;

    bool isLand(Card* card);
    bool isCreature(Card* card);

    CardButton* createCardButton(Card* card);


    //For mana zone
    void showLandPopup(ManaType type, ZoneLayout layout);
    void updateManaButton(ManaType type, ZoneLayout layout);


    // FOR TESTING PURPOSES
    void setupHand();

};


#endif // MAINWINDOW_H
