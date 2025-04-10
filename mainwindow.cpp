#include "mainwindow.h"
#include "type.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Player *player)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , player(player)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}
