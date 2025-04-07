#include "mainwindow.h"
#include "player.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Player player;
    MainWindow w(nullptr, &player);
    w.show();
    return a.exec();
}
