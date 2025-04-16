#include "mainwindow.h"
#include "cardapimanager.h"
#include "ui_mainwindow.h"
#include "carddictionary.h"
#include "gamestate.h"
#include "phase.h"
#include "textparser.h"
#include <QDebug>
#include <QMouseEvent>

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


CardButton::CardButton(Card* card, QWidget* parent)
    : QPushButton(parent), cardPtr(card) {
    cardName = card->name;
    setCheckable(true);

    setStyleSheet(R"(
        QPushButton {
            background-color: white;
            border: 1px solid #333;
            padding: 0px;
        }
        QPushButton:checked {
            background-color: lightblue;
            border: 2px solid blue;
        }
        )");

    updateVisual();
}

void CardButton::updateVisual() {
    if (cardPtr) {
        setText(cardPtr->name);
        // setStyleSheet("border: 1px solid #333; background-color: white; padding: 0px;");
    }
}

void MainWindow::cardMovedFromLibray(Card* card, QString zone){
    CardButton* cardButton = new CardButton(card);
    // cardButton->setStyleSheet("border: 1px solid #333; background-color: white;");
    cardButton->setFixedSize(100, 140);

    if(zone == "hand"){
        connect(apiManager, &CardAPIManager::cardFetched, cardButton, &CardButton::updateCard);
        apiManager->fetchCardByName(card->name);
        ui->PlayerHand->addWidget(cardButton, 0, ui->PlayerHand->count(), Qt::AlignCenter);
    }
}

void CardButton::updateCard(const Card &card){
    if (card.name != cardName) return; // Confirm it's the correct card

    cardDictionary::addCard(card);
    Card elfCard = cardDictionary::getCard(card.name);

    //     // ui->imageLabel->setPixmap(QPixmap::fromImage(elfCard.image).scaled(
    //     //     ui->imageLabel->size(),
    //     //     Qt::KeepAspectRatio,
    //     //     Qt::SmoothTransformation));


     QPixmap pixmap = QPixmap::fromImage(elfCard.image).scaled(
        this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    this->setIcon(QIcon(pixmap));
    this->setText("");
    this->setIconSize(pixmap.rect().size());
}

void MainWindow::attackPhase(Player* player){


    if (player->isActivePlayer){
        ui->playCardButton->setText("Select Attackers...");
    }
    else {
        ui->playCardButton->setText("Enemy Selecting Attackers");
        ui->playCardButton->setDisabled(true);
    }
}

void MainWindow::collectAttackers(Player* player){

    QGridLayout* battlefield;
    if(player->isActivePlayer){
        battlefield = ui->playerContainer->battlefield;
    }
    else{
        battlefield = ui->enemyContainer->battlefield;
    }

    for(int i = 0; i < battlefield->count(); i++){

        QLayoutItem* item = battlefield->itemAt(i);
        QWidget* widget = item->widget();

        if(CardButton* button = qobject_cast<CardButton*>(widget)){
            Card* card = button->cardPtr;
            combatants[card];
        }
    }
}

void MainWindow::collectBlockers(Player* player){

}

void MainWindow::updateUI(Player* player){
    QVector<Zone*> zones = player->getZones();
    QGridLayout* container;

    // Set the pointer to the right Container
    if(player->playerID == 0){
        container = ui->playerContainer;
    }
    else{
        container = ui->enemyContainer;
    }

    // Go through all containers and update UI
    for (Zone* zone : zones){
        if (zone->type == ZoneType::HAND){
            container = container->hand;
            updateZone(container, zone);
        }
        else if (zone->type == ZoneType::BATTLEFIELD){
            container = container->battlefield;
            updateZone(container, zone);
        }
        else if (zone->type == ZoneType::GRAVEYARD){
            container = container->graveyard;
            updateZone(container, zone);
        }
        else if (zone->type == ZoneType::EXILE){
            container = container->exile;
            updateZone(container, zone);
        }
    }
}

void MainWindow::updateZone(QGridLayout* container, Zone* zone){

    // Clear Zone
    QLayoutItem* item;
    while((item = container->takeAt(0)) != nullptr){
        if (item->widget()){
            delete item->widget();
        }
        delete item;
    }

    int i = 0;

    for(Card* card : *zone){
        CardButton* cardButton = new CardButton(card);
        int row = i / container->rowCount();
        int column = i % container->columnCount();
        container->addWidget(cardButton, row, column, Qt::AlignCenter);
        i++;
    }
}

void MainWindow::toggleButton(){
    CardButton* button = qobject_cast<CardButton*>(sender());
    if(!button)
        return;

    if(statePointer->currentPhase == Phase::DeclareAttackers ||
        statePointer->currentPhase == Phase::DeclareBlockers){
        if(selectedCards.contains(button)){
            selectedCards.removeOne(button);
            button->setChecked(false);
        }
        else{
            selectedCards.append(button);
            button->setChecked(true);
        }
    }
    else{
        if (currentCard && currentCard != button){
            currentCard->setChecked(false);
        }
        // Set current card to button if checked
        currentCard = button->isChecked() ? button : nullptr;
    }

}

void MainWindow::clearSelection(){

    selectedCards.clear();
    currentCard = nullptr;

    for(CardButton* card : activeCards){
        card->setChecked(false);
    }

}

void CardButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "CardButton clicked:" << cardName;
    }
    QPushButton::mousePressEvent(event);  // Keep original click signal
}


