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
    statePointer = game->state;
    userPlayer = statePointer->player1;

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
        ui->enemyGraveyard,
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

    connect(this, &MainWindow::sendCombatCards, game, &gamemanager::onCombatCardsReceived);
}

MainWindow::~MainWindow() {
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
        ui->playerHand->addWidget(cardButton, 0, ui->playerHand->count(), Qt::AlignCenter);
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

        handlePhase();

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

        if(card->isLand){
            manaLayout[card->color].append(cardButton);
            break;
        }

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

void MainWindow::handlePhase(){
    PhaseRules rules = statePointer->getPhaseRules();
    bool isActive = userPlayer->isActivePlayer;

    for (CardButton* button : activeCards){
        Card* card = button->cardPtr;
        bool shouldEnable = false;

        if(rules.canPlayInstant && card->type == CardType::INSTANT){
            shouldEnable = true;
        }
        if(rules.canPlaySorcery && card->type == CardType::SORCERY){
            shouldEnable = true;
        }
        if(isActive && rules.canDeclareAttack && card->type == CardType::CREATURE){
            shouldEnable = true;
        }
        if(!isActive && rules.canDeclareDefense && card->type == CardType::CREATURE){
            shouldEnable = true;
        }
        if(isActive && rules.canUntap && card->isLand && card->isTapped){
            shouldEnable = true;
        }
        if(isActive && rules.canPlaySorcery &&
                (card->type == CardType::LAND ||
                 card->type == CardType::CREATURE ||
                 card->type == CardType::ARTIFACT ||
                 card->type == CardType::ENCHANTMENT ||
                 card->type == CardType::PLANESWALKER ||
                 card->type == CardType::BATTLE)) {
            shouldEnable = true;
        }

        button->setEnabled(shouldEnable);
    }
}

void MainWindow::startTargeting(){
    isTargeting = true;
    // IMPLEMENT ISTARGETING IN CARD SELECTION
    selectedCards.empty();

    for(CardButton* button : activeCards){
        button->setEnabled(false);
    }

    QGridLayout* targetZone = ui->enemyBattlefield;

    for(int i = 0; i < targetZone->count(); i++){

        QLayoutItem* item = targetZone->itemAt(i);
        QWidget* widget = item->widget();
        CardButton* button = qobject_cast<CardButton*>(widget);
        button->setEnabled(true);
    }
}

void MainWindow::showAllCards(){
    return;
}
