#include "mainwindow.h"
#include "type.h"
#include "ui_mainwindow.h"
#include "carddictionary.h"

MainWindow::MainWindow(QWidget *parent, Player *player)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , player(player)
{
    ui->setupUi(this);

    CardAPIManager *apiManager = new CardAPIManager(this);

    connect(apiManager, &CardAPIManager::cardFetched,
            this, &carddictionary::addCard);

        apiManager->fetchCardByName("Lightning Bolt");

    connect(ui->healthButton, &QPushButton::clicked, this, [&]() {
        int value = ui->testSpinBox->value();
        emit uiTestHealth(value);
        qDebug() << "Health button Clicked " << value;
    });

    connect(ui->damageButton, &QPushButton::clicked, this, [&]() {
        int value = ui->testSpinBox->value();
        emit uiTestDamage(value);
        qDebug() << "Damage Button Clicked " << value;
    });

    connect(this, &MainWindow::uiTestHealth, player, &Player::gainLife);
    connect(this, &MainWindow::uiTestDamage, player, &Player::takeDamage);

    connect(ui->addManaButton, &QPushButton::clicked, this, &MainWindow::manaAddTest);
    connect(ui->payManaButton, &QPushButton::clicked, this, &MainWindow::manaPayTest);

    connect(ui->healthSpinBox, &QSpinBox::valueChanged, ui->healthSlider, &QSlider::setValue);

    connect(this, &MainWindow::uiTestManaAdd, player, &Player::addMana);
}

MainWindow::~MainWindow()
{
    delete ui;
}
