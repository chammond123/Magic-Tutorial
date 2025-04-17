#include "mainwindow.h"
#include "cardapimanager.h"
#include "ui_mainwindow.h"
#include "carddictionary.h"
#include "gamestate.h"
#include "gamemanager.h"
#include "phase.h"
#include "textparser.h"
#include <QDebug>
#include <QtGui/qevent.h>

MainWindow::MainWindow(gamemanager* game, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Magic Tutorial");
    apiManager = new CardAPIManager(this);
    userPlayer = statePointer->player1;


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

    connect(this, MainWindow::sendCombatCards, game, gamemanager::OnCombatantCardsReceived);
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
        ui->hand->addWidget(cardButton, 0, ui->hand->count(), Qt::AlignCenter);
    }
}

void MainWindow::handleCardSelected(CardButton* clicked) {
    if (currentSelectedCard && currentSelectedCard != clicked)
        currentSelectedCard->setSelected(false);

    currentSelectedCard = clicked;
    currentSelectedCard->setSelected(true);
}

void MainWindow::attackPhase(){
    if (userPlayer->isActivePlayer){
        ui->playCardButton->setText("Select Attackers...");
    }
    else {
        ui->playCardButton->setText("Enemy Selecting Attackers");
        ui->playCardButton->setDisabled(true);
    }
}

void MainWindow::collectAttackers(){

    QGridLayout* battlefield;

    if(userPlayer->isActivePlayer){
        battlefield = ui->playerContainer->battlefield;
    }
    else{
        battlefield = ui->enemyContainer->battlefield;
    }

    for(int i = 0; i < battlefield->count(); i++){

        QLayoutItem* item = battlefield->itemAt(i);
        QWidget* widget = item->widget();
        CardButton* button = qobject_cast<CardButton*>(widget);

        if(button->isChecked()){
            buttonCombatants[button];
        }
    }

    targetIt = buttonCombatants.begin();
}

void MainWindow::collectBlockers(){


    if (targetIt == buttonCombatants.end()){
        extractCombatants(buttonCombatants);
        return;
    }

    targetIt.value() = selectedButtons;
    targetIt++;
}

void MainWindow::updateUI(){

    for (int i = 0; i < 2; i++){
        QGridLayout* container;
        QVector<Zone*> zones;
        if (i == 0){
            zones = userPlayer->getZones();
            container = ui->playerContainer;
        }
        else{
            zones = enemyPlayer->getZones();
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

// void MainWindow::toggleButton(){
//     CardButton* button = qobject_cast<CardButton*>(sender());
//     if(!button)
//         return;

//     if(statePointer->currentPhase == Phase::DeclareAttackers ||
//         statePointer->currentPhase == Phase::DeclareBlockers){
//         if(selectedCards.contains(button)){
//             selectedCards.removeOne(button);
//             button->setChecked(false);
//         }
//         else{
//             selectedCards.append(button);
//             button->setChecked(true);
//         }
//     }
//     else{
//         if (currentCard && currentCard != button){
//             currentCard->setChecked(false);
//         }
//         // Set current card to button if checked
//         currentCard = button->isChecked() ? button : nullptr;
//     }
// }

void MainWindow::clearSelection(){

    selectedButtons.clear();
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

void MainWindow::extractCombatants(QMap<CardButton*, QVector<CardButton*>> packedCombatCards){

    for(auto it = packedCombatCards.begin(); it != packedCombatCards.end(); it++){

        Card* attacker = it.key()->cardPtr;
        combatants[attacker];

        for (auto button : it.value()){

            Card* blocker = button->cardPtr;
            combatants[attacker].append(blocker);
        }
    }

    emit sendCombatCards(combatants);
}
