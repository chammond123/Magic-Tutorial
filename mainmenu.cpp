#include "mainmenu.h"
#include "ui_mainmenu.h"
#include <QPushButton>

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    setWindowTitle("Magic Tutorial");

    connect(ui->playButton, &QPushButton::clicked, this, &MainMenu::startGame);
    connect(ui->aboutButton, &QPushButton::clicked, this, &MainMenu::openAboutPage);
    connect(ui->tutorialButton, &QPushButton::clicked, this, &MainMenu::openTutorialPage);
}

MainMenu::~MainMenu() {
    delete ui;
}
