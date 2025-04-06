#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, Player* player = nullptr);
    ~MainWindow();

public slots:
    void receiveMana(QMap<ManaColor, int>* manaPool);
    void manaAddTest();
    void manaPayTest();

signals:
    void uiTestHealth(int health);
    void uiTestDamage(int damage);
    void uiTestManaAdd(QMap<ManaColor, int>* mana);
    void uiTestManaPay(QMap<ManaColor, int>* mana);

private:
    Ui::MainWindow *ui;
    Player* player;
};
#endif // MAINWINDOW_H
