#include "mainwindow.h"
#include "cardapimanager.h"
#include "ui_mainwindow.h"
#include "carddictionary.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Magic Tutorial");

    CardAPIManager *apiManager = new CardAPIManager(this);


    connect(apiManager, &CardAPIManager::errorOccurred, this, [](const QString &error) {
        qDebug() << "API Error:" << error;
    });

    connect(apiManager, &CardAPIManager::cardFetched, this, [=](const Card &card) {
        carddictionary::addCard(card);
        qDebug() << carddictionary::getCard("Lightning Bolt").description;
        QMap<ManaType, int> manaCost = carddictionary::getCard("Lightning Bolt").cost;
        for (auto [color, value] : manaCost.toStdMap()){
            QString colorString = manaTypeToString(color);
            qDebug() << colorString << " : " << value;
        }
        ui->imageLabel->setPixmap(QPixmap::fromImage(carddictionary::getCard("Lightning Bolt").image).scaled(
            ui->imageLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
    });

    apiManager->fetchCardByName("Lightning Bolt");

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::manaTypeToString(ManaType type) {
    switch (type) {
    case ManaType::RED:   return "Red";
    case ManaType::BLUE:  return "Blue";
    case ManaType::GREEN: return "Green";
    case ManaType::BLACK: return "Black";
    case ManaType::WHITE: return "White";
    default:              return "Unknown";
    }
}
