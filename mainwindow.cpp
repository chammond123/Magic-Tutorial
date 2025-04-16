#include "mainwindow.h"
#include "cardapimanager.h"
#include "ui_mainwindow.h"
#include "carddictionary.h"
#include "textparser.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Magic Tutorial");
    apiManager = new CardAPIManager(this);


    connect(apiManager, &CardAPIManager::errorOccurred, this, [](const QString &error) {
        qDebug() << "API Error:" << error;
    });

    connect(apiManager, &CardAPIManager::cardFetched, this, [=](const Card &card) {
        cardDictionary::addCard(card);
        Card test = cardDictionary::getCard(card.name);
        qDebug() << "---";
        qDebug() << card.name;
        qDebug() << test.description;

        QMap<ManaType, int> manaCost = test.cost;
        for (auto [color, value] : manaCost.toStdMap()){
            QString colorString = manaTypeToString(color);
            qDebug() << colorString << " : " << value;
        }
        // ui->imageLabel->setPixmap(QPixmap::fromImage(elfCard.image).scaled(
        //     ui->imageLabel->size(),
        //     Qt::KeepAspectRatio,
        //     Qt::SmoothTransformation));

        qDebug() << test.toughness;
        qDebug() << test.power;
        qDebug() << "---";
    });



    for(QString cardName : TextParser::getListFromText(QFile(":/text/additional_files/deck.txt"))){
        qDebug() << cardName;
        apiManager->fetchCardByName(cardName);
    }

    Card* test = new Card("Elspeth's Devotee");
    Card* test1 = new Card("Black Lotus");
    Card* test2 = new Card("Llanowar Elves");
    cardMovedFromLibray(test, "hand");
    cardMovedFromLibray(test1, "hand");
    cardMovedFromLibray(test2, "hand");
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


void MainWindow::cardMovedFromLibray(Card* card, QString zone){
    CardButton* cardButton = new CardButton(card);
    connect(cardButton, &CardButton::cardSelected, this, &MainWindow::handleCardSelected);
    cardButton->setFixedSize(100, 140);

    if(zone == "hand"){
        connect(apiManager, &CardAPIManager::cardFetched, cardButton, &CardButton::updateCard);
        apiManager->fetchCardByName(card->name);
        ui->PlayerHand->addWidget(cardButton, 0, ui->PlayerHand->count(), Qt::AlignCenter);
    }
}

void MainWindow::handleCardSelected(CardButton* clicked) {
    if (currentSelectedCard && currentSelectedCard != clicked)
        currentSelectedCard->setSelected(false);

    currentSelectedCard = clicked;
    currentSelectedCard->setSelected(true);
}


