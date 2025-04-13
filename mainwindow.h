#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "type.h"
#include "zone.h"
#include "card.h"
#include "cardapimanager.h"
#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CardButton : public QPushButton{
    Q_OBJECT
public:
    Card* cardPtr;
    QString cardName;

    explicit CardButton(Card* card, QWidget* parent = nullptr);
    void updateVisual();

public slots:
    void updateCard(const Card &card);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    void cardZoneChanged(Card*, QString zone);


signals:


private:
    Ui::MainWindow *ui;
    CardAPIManager* apiManager;
    QString manaTypeToString(ManaType type);
};
#endif // MAINWINDOW_H
