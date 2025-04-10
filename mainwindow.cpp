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

void MainWindow::receiveMana(QMap<ManaType, int> *manaPool)
{
    for (auto [color, amount] : manaPool->toStdMap()) {
        if (color == ManaType::WHITE) {
            ui->whiteManaSpinBox->setValue(amount);
        } else if (color == ManaType::BLACK) {
            ui->blackManaSpinBox->setValue(amount);
        } else if (color == ManaType::BLUE) {
            ui->blueManaSpinBox->setValue(amount);
        } else if (color == ManaType::RED) {
            ui->redManaSpinBox->setValue(amount);
        } else if (color == ManaType::GREEN) {
            ui->greenManaSpinBox->setValue(amount);
        }
    }
}

void MainWindow::manaAddTest()
{
    QMap<ManaType, int> manaCosts;

    QString inputColor = ui->comboBox->currentText();
    int value = ui->testSpinBox->value();

    qDebug() << inputColor << " clicked with " << value << " amount";

    if (inputColor == "White") {
        manaCosts[ManaType::WHITE] = value;
    } else if (inputColor == "Black") {
        manaCosts[ManaType::BLACK] = value;
    } else if (inputColor == "Blue") {
        manaCosts[ManaType::BLUE] = value;
    } else if (inputColor == "Red") {
        manaCosts[ManaType::RED] = value;
    } else if (inputColor == "Green") {
        manaCosts[ManaType::GREEN] = value;
    }

    emit uiTestManaAdd(&manaCosts);
}

void MainWindow::manaPayTest()
{
    QMap<ManaType, int> manaCosts;

    QString inputColor = ui->comboBox->currentText().trimmed();
    int value = ui->testSpinBox->value();

    qDebug() << inputColor << "clicked with" << value << " amount";
    if (inputColor == "White") {
        manaCosts[ManaType::WHITE] = value;
        qDebug() << "White emit sent";
    } else if (inputColor == "Black") {
        manaCosts[ManaType::BLACK] = value;
    } else if (inputColor == "Blue") {
        manaCosts[ManaType::BLUE] = value;
    } else if (inputColor == "Red") {
        manaCosts[ManaType::RED] = value;
    } else if (inputColor == "Green") {
        manaCosts[ManaType::GREEN] = value;
    }

    emit uiTestManaPay(&manaCosts);
}
