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
#include <QMessageBox>
#include <type.h>

MainWindow::MainWindow(gamemanager* game, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Magic Tutorial");

    ui->CardDescription->setReadOnly(true);
    ui->CardDescription->setFocusPolicy(Qt::NoFocus);

    apiManager = new CardAPIManager(this);
    userPlayer = nullptr;



    playerLayout = {
        ui->playerHand,
        ui->playerBattlefield,
        ui->playerGraveyard,
        ui->playerExile,
        ui->playerRed,
        ui->playerGreen,
        ui->playerBlue,
        ui->playerWhite,
        ui->playerBlack,
        ui->PlayerHealth
    };

    enemyLayout = {
        nullptr,
        ui->enemyBattlefield,
        ui->enemyDeck,
        ui->enemyExile,
        ui->enemyRed,
        ui->enemyGreen,
        ui->enemyBlue,
        ui->enemyWhite,
        ui->enemyBlack,
        ui->EnemyHealth
    };


    connect(apiManager, &CardAPIManager::errorOccurred, this, [](const QString &error) {
        qDebug() << "API Error:" << error;
    });

    // connect(ui->playCardButton, &QPushButton::clicked, this, &MainWindow::on_playCardButton_clicked);

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
    Card* test3 = new Card("Mountain");
    cardMovedFromLibray(test, "hand");
    cardMovedFromLibray(test1, "hand");
    cardMovedFromLibray(test2, "hand");
    cardMovedFromLibray(test3, "hand");

    // connect(this, MainWindow::sendCombatCards, game, gamemanager::OnCombatantCardsReceived);
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

bool MainWindow::isLand(Card* card){
    return card->type == CardType::LAND;
}

bool MainWindow::isCreature(Card* card){
    return card->type == CardType::CREATURE;
}

void MainWindow::on_playCardButton_clicked(){
    // Just for testing need to handle more condiction phrases, cost, priority

    //emit playcard(player Pointer, currentCard, nullptr)

    qDebug() << "playCardButton called";
    qDebug() << "played card: " + currentSelectedCard->cardName;

    if (!currentSelectedCard) {
        QMessageBox::information(this, "No card selected", "Select a card to play.");
        return;
    }

    Card* card = currentSelectedCard->cardPtr;

    if(card->isLand){
        ManaType manaType = card->color;
        landGroups[manaType].append(currentSelectedCard);

        ui->playerHand->removeWidget(currentSelectedCard);
        currentSelectedCard->setParent(nullptr);
        currentSelectedCard->deleteLater();
        currentSelectedCard->setSelected(false);
        currentSelectedCard = nullptr;

        qDebug() << "Played land to mana stack.";


    }

    else if (card->isPermanent){
        // need to handle mana cost and something else
        ui->playerBattlefield->addWidget(currentSelectedCard);
        ui->playerHand->removeWidget(currentSelectedCard);

        currentSelectedCard->setSelected(false);
        currentSelectedCard = nullptr;

        qDebug() << "Played creature to battlefield.";
    }
    else {
        QMessageBox::warning(this, "Can't play card", "This type of card can't be played directly.");
    }
}


void MainWindow::cardMovedFromLibray(Card* card, QString zone){
    CardButton* cardButton = new CardButton(card);
    connect(cardButton, &CardButton::cardSelected, this, &MainWindow::handleCardSelected);
    connect(cardButton, &CardButton::hovered, this, &MainWindow::updateMagnifier);
    cardButton->setFixedSize(100, 140);

    if(zone == "hand"){
        connect(apiManager, &CardAPIManager::cardFetched, cardButton, &CardButton::updateCard);
        apiManager->fetchCardByName(card->name);
        ui->playerHand->addWidget(cardButton, 0, ui->playerHand->count(), Qt::AlignCenter);
    }
}

void MainWindow::handleCardSelected(CardButton* clicked) {
    // if(statePointer->currentPhase == Phase::DeclareAttackers ||
    //     statePointer->currentPhase == Phase::DeclareBlockers){
    //     if(selectedButtons.contains(clicked)){
    //         selectedButtons.removeOne(clicked);
    //         clicked->setSelected(false);
    //     }
    //     else{
    //         selectedButtons.append(clicked);
    //         clicked->setSelected(true);
    //     }
    // }

    if(currentSelectedCard == nullptr){
        currentSelectedCard = clicked;
        currentSelectedCard->setSelected(true);
    }
    else if (currentSelectedCard && currentSelectedCard != clicked){
        currentSelectedCard->setSelected(false);
        currentSelectedCard = clicked;
        currentSelectedCard->setSelected(true);
    }
    else if (currentSelectedCard == clicked){
        currentSelectedCard->setSelected(false);
        currentSelectedCard = nullptr;
    }
}

void MainWindow::updateMagnifier(Card* card) {
    if (!card) return;

    // Scale and display the image
    Card cardFromDictionary = cardDictionary::getCard(card->name);

    QPixmap pixmap = QPixmap::fromImage(cardFromDictionary.image).scaled(
        ui->MagnifierImage->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    ui->MagnifierImage->setPixmap(pixmap);

    // Show the card description
    ui->CardDescription->setText(cardFromDictionary.description);
}

void MainWindow::attackPhase(){
    if (userPlayer->isActivePlayer){
        ui->playCardButton->setText("Select Attackers...");
    }
    else {
        ui->playCardButton->setText("Enemy Selecting");
        ui->playCardButton->setDisabled(true);
    }
}

void MainWindow::collectAttackers(){

    QGridLayout* battlefield;

    if(userPlayer->isActivePlayer){
        battlefield = ui->playerBattlefield;
    }
    else{
        battlefield = ui->enemyBattlefield;
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

    QVector<Zone*> zones;
    ZoneLayout layout;
    Player* currPlayer;

    for (int i = 0; i < 2; i++){

        // Set the zones, layout, and player
        if (i == 0){
            zones = userPlayer->getZones();
            layout = playerLayout;
            currPlayer = userPlayer;
        }
        else{
            zones = enemyPlayer->getZones();
            layout = enemyLayout;
            currPlayer = enemyPlayer;
        }

        // Go through all zones and update containers
        for (Zone* zone : zones){
            if (zone->type == ZoneType::HAND){
                if(layout.hand){
                    updateZone(layout.hand, zone);
                }
            }
            else if (zone->type == ZoneType::BATTLEFIELD){
                updateZone(layout.battlefield, zone);
            }
            else if (zone->type == ZoneType::GRAVEYARD){
                Card* card = zone->drawTop();
                QPixmap image = QPixmap::fromImage(card->image);
                layout.graveyard->setPixmap(image.scaled(layout.graveyard->size()));
            }
            else if (zone->type == ZoneType::EXILE){
                Card* card = zone->drawTop();
                QPixmap image = QPixmap::fromImage(card->image);
                layout.exile->setPixmap(image.scaled(layout.exile->size()));
            }
        }

        // Set the Mana
        for (auto [color, amount] : currPlayer->manaPool.toStdMap()){
            switch (color) {
            case ManaType::RED:   layout.red->setText(QString::number(amount));
            case ManaType::BLUE:  layout.blue->setText(QString::number(amount));
            case ManaType::GREEN: layout.green->setText(QString::number(amount));
            case ManaType::BLACK: layout.black->setText(QString::number(amount));
            case ManaType::WHITE: layout.white->setText(QString::number(amount));
            default:              break;
            }
        }

        // Set the Health
        layout.health->setText(QString::number(currPlayer->health));

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
    // currentCard = nullptr;

    for(CardButton* card : activeCards){
        card->setChecked(false);
    }

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


