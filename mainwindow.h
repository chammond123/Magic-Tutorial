#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "zone.h"
#include "player.h"
#include "card.h"
#include "cardapimanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, Player *player = nullptr);
    ~MainWindow();

public slots:


signals:


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
