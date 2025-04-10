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
        cardDictionary::addCard(card);
        Card elfCard = cardDictionary::getCard("Elspeth's Devotee");

        qDebug() << elfCard.description;

        QMap<ManaType, int> manaCost = elfCard.cost;
        for (auto [color, value] : manaCost.toStdMap()){
            QString colorString = manaTypeToString(color);
            qDebug() << colorString << " : " << value;
        }
        ui->imageLabel->setPixmap(QPixmap::fromImage(elfCard.image).scaled(
            ui->imageLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));

        qDebug() << elfCard.toughness;
        qDebug() << elfCard.power;
    });

    apiManager->fetchCardByName("Lightning Bolt");
    apiManager->fetchCardByName("Llanowar Elves");
    apiManager->fetchCardByName("Elspeth's Devotee");

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
    case ManaType::COLORLESS: return "Colorless";
    case ManaType::ANY: return "Any";
    default:              return "Unknown";
    }
}
