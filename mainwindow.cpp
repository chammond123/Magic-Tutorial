#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "type.h"

MainWindow::MainWindow(QWidget *parent, Player* player)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    player(player)
{
    ui->setupUi(this);

    connect(ui->healthButton, &QPushButton::clicked, this, [&](){
        int value = ui->testSpinBox->value();
        emit uiTestHealth(value);
        qDebug() << "Health button Clicked " << value;
    });

    connect(ui->damageButton,  &QPushButton::clicked, this, [&](){
        int value = ui->testSpinBox->value();
        emit uiTestDamage(value);
        qDebug() << "Damage Button Clicked " << value;
    });

    connect(this, &MainWindow::uiTestHealth, player, &Player::gainLife);
    connect(this, &MainWindow::uiTestDamage, player, &Player::takeDamage);

    connect(ui->addManaButton, &QPushButton::clicked, this, &MainWindow::manaAddTest);
    connect(ui->payManaButton, &QPushButton::clicked, this, &MainWindow::manaPayTest);

    connect(ui->healthSpinBox, &QSpinBox::valueChanged, ui->healthSlider, &QSlider::setValue);
    connect(player, &Player::healthChanged, ui->healthSpinBox, &QSpinBox::setValue);
    connect(player, &Player::manaPoolChanged, this, &MainWindow::receiveMana);

    connect(this, &MainWindow::uiTestManaAdd, player, &Player::addMana);
    connect(this, &MainWindow::uiTestManaAdd, player, &Player::useMana);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receiveMana(QMap<ManaColor, int>* manaPool){
    for(auto[color, amount] : manaPool->toStdMap()){
        if(color == ManaColor::White){
            ui->whiteManaSpinBox->setValue(amount);
        }
        else if(color == ManaColor::Black){
            ui->blackManaSpinBox->setValue(amount);
        }
        else if(color == ManaColor::Blue){
            ui->blueManaSpinBox->setValue(amount);
        }
        else if(color == ManaColor::Red){
            ui->redManaSpinBox->setValue(amount);
        }
        else if(color == ManaColor::Green){
            ui->greenManaSpinBox->setValue(amount);
        }
    }
}

void MainWindow::manaAddTest(){
    QMap<ManaColor, int> manaCosts;

    QString inputColor = ui->comboBox->currentText();
    int value = ui->testSpinBox->value();

    qDebug() << inputColor << " clicked with " << value << " amount";

    if (inputColor == "White"){
        manaCosts[ManaColor::White] = value;
    } else if (inputColor == "Black"){
        manaCosts[ManaColor::Black] = value;
    } else if (inputColor == "Blue"){
        manaCosts[ManaColor::Blue] = value;
    } else if (inputColor == "Red"){
        manaCosts[ManaColor::Red] = value;
    } else if (inputColor == "Green"){
        manaCosts[ManaColor::Green] = value;
    }

    emit uiTestManaAdd(&manaCosts);
}

void MainWindow::manaPayTest(){
    QMap<ManaColor, int> manaCosts;

    QString inputColor = ui->comboBox->currentText().trimmed();
    int value = ui->testSpinBox->value();

    qDebug() << inputColor << "clicked with" << value << " amount";
    if (inputColor == "White"){
        manaCosts[ManaColor::White] = value;
        qDebug() << "White emit sent";
    } else if (inputColor == "Black"){
        manaCosts[ManaColor::Black] = value;
    } else if (inputColor == "Blue"){
        manaCosts[ManaColor::Blue] = value;
    } else if (inputColor == "Red"){
        manaCosts[ManaColor::Red] = value;
    } else if (inputColor == "Green"){
        manaCosts[ManaColor::Green] = value;
    }

    emit uiTestManaPay(&manaCosts);
}
