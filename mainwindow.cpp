#include "mainwindow.h"
#include "cardapimanager.h"
#include "ui_mainwindow.h"
#include "carddictionary.h"
#include "gamemanager.h"
#include "textparser.h"
#include <QTimer>
#include <QDebug>
#include <QtGui/qevent.h>
#include <QMessageBox>
#include <type.h>
#include <QScrollArea>

MainWindow::MainWindow(gamemanager* game, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
    , user(":/text/additional_files/deck.txt")
{
    ui->setupUi(this);
    setWindowTitle("Magic Tutorial");

    ui->CardDescription->setReadOnly(true);
    ui->CardDescription->setFocusPolicy(Qt::NoFocus);

    apiManager = new CardAPIManager(this);
    // TESTING: SWITCH BACK TO STATE POINTER PLAYER
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

    connect(ui->playerRedIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::RED);
    });

    connect(ui->playerWhiteIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::WHITE);
    });

    connect(ui->playerGreenIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::GREEN);
    });

    connect(ui->playerBlueIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::BLUE);
    });

    connect(ui->playerBlackIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::BLACK);
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

        // if (cardDictionary::dict.size() == 11 && createFlag){
        //     qDebug() << "SETUP CALLED";
        //     setupHand();
        //     createFlag = false;
        // }
    });

    for(QString cardName : TextParser::getListFromText(QFile(":/text/additional_files/deck.txt"))){
        qDebug() << cardName;
        apiManager->fetchCardByName(cardName);
    }

    QTimer::singleShot(1000, this, [=](){
        qDebug() << "SETUP CALLED";
        setupHand();
    });

    // connect(this, MainWindow::sendCombatCards, game, gamemanager::OnCombatantCardsReceived);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//FOR TESTING
void MainWindow::setupHand(){

    userPlayer = &user;

    card1 = cardDictionary::getCard("Lightning Bolt");
    qDebug() << "CARD NAME: " << card1.name;

    card2 = cardDictionary::getCard("Coral Merfolk");
    card3 = cardDictionary::getCard("Goblin Bully");
    card4 = cardDictionary::getCard("Mountain");
    card5 = cardDictionary::getCard("Mountain");
    card6 = cardDictionary::getCard("Mountain");
    card7 = cardDictionary::getCard("Mountain");

    // Card* test = &card1;
    // Card* test1 = &card2;
    // Card* test2 = &card3;
    // Card* test3 = &card4;
    if(userPlayer){
        qDebug() << "Found User!";
    }
    else {
        return;
    }

    userPlayer->Hand.addCard(&card1, false);
    userPlayer->Hand.addCard(&card2, false);
    userPlayer->Hand.addCard(&card3, true);
    userPlayer->Hand.addCard(&card4, false);
    userPlayer->Hand.addCard(&card5, false);
    userPlayer->Hand.addCard(&card6, false);
    userPlayer->Hand.addCard(&card7, false);

    qDebug() << "Added all cards to Library";

    updateUI();
    // cardMovedFromLibrary(test, "hand");
    // cardMovedFromLibrary(test1, "hand");
    // cardMovedFromLibrary(test2, "hand");
    // cardMovedFromLibrary(test3, "hand");
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

void MainWindow::showLandPopup(ManaType manaType){
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Lands: " + manaTypeToString(manaType));
    dialog->setMinimumSize(400, 300);

    QScrollArea* scrollArea = new QScrollArea(dialog);
    QWidget* container = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(container);

    qDebug() << "Mana stack size:" << landGroups[manaType].size();
    qDebug() << manaTypeToString(manaType);

    for (CardButton* land : landGroups[manaType]) {
        qDebug() << "not added";
        layout->addWidget(land);
        qDebug() << "added";
    }


    qDebug() << "a";

    container->setLayout(layout);
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
    mainLayout->addWidget(scrollArea);
    dialog->setLayout(mainLayout);
    dialog->exec();  // modal
}

void MainWindow::on_playCardButton_clicked(){
    // Just for testing need to handle more condiction phrases, cost, priority

    //emit playcard(player Pointer, currentCard, nullptr)

    if (!currentSelectedCard) {
        QMessageBox::information(this, "No card selected", "Select a card to play.");
        return;
    }

    qDebug() << "playCardButton called";
    qDebug() << "played card: " + currentSelectedCard->cardName;

    Card* card = currentSelectedCard->cardPtr;
    qDebug() << "got card " << card->name;

    qDebug() << ui->playerHand->count();

    if(card->type == CardType::LAND){
        userPlayer->moveCardZone(card, userPlayer->Hand, userPlayer->Battlefield, false);

        // ManaType mana = card->color;
        // qDebug() << manaTypeToString((mana));

        currentSelectedCard->setParent(this);

        // Add to land group
        landGroups[ManaType::RED].append(currentSelectedCard);

        currentSelectedCard->setChecked(false);
        currentSelectedCard = nullptr;

        qDebug() << "Played land to mana stack.";
        qDebug() << ui->playerHand->count();
    }

    else if (card->isPermanent){
        // FOR TESTING PURPOSES
        userPlayer->moveCardZone(card, userPlayer->Hand, userPlayer->Battlefield, false);

        currentSelectedCard->setChecked(false);
        currentSelectedCard = nullptr;

        qDebug() << "Played creature to battlefield.";
    }
    else {
        userPlayer->moveCardString(card, "hand", "graveyard", true);
    }
    updateUI();
}


void MainWindow::cardMovedFromLibrary(Card* card, QString zone){
    CardButton* cardButton = new CardButton(card);
    connect(cardButton, &CardButton::cardSelected, this, &MainWindow::handleCardSelected);
    connect(cardButton, &CardButton::hovered, this, &MainWindow::updateMagnifier);
    cardButton->setFixedSize(100, 140);

    if(zone == "hand"){
        // connect(apiManager, &CardAPIManager::cardFetched, cardButton, &CardButton::updateCard);
        // apiManager->fetchCardByName(card->name);
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
        currentSelectedCard->setChecked(true);
    }
    else if (currentSelectedCard != clicked){
        currentSelectedCard->setChecked(false);
        currentSelectedCard = clicked;
        currentSelectedCard->setChecked(true);
    }
    else if (currentSelectedCard->isChecked()){
        currentSelectedCard->setChecked(false);
        currentSelectedCard = nullptr;
    }

    qDebug() << clicked->cardName << " is Checked: " << clicked->isChecked();
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

    qDebug() << "updateUI called";
    QVector<Zone*> zones;
    ZoneLayout layout;
    Player* currPlayer;

    for (int i = 0; i < 1; i++){

        // Set the zones, layout, and player
        if (i == 0){
            zones = userPlayer->getZones();
            layout = playerLayout;
            currPlayer = userPlayer;
        }
        // else{
        //     zones = enemyPlayer->getZones();
        //     layout = enemyLayout;
        //     currPlayer = enemyPlayer;
        // }
        if(!currPlayer){
            qDebug() << "Lost Player";
            break;
        }

        // Go through all zones and update containers
        for (Zone* zone : zones){
            if (zone->type == ZoneType::HAND){
                updateZone(layout.hand, zone);
            }
            else if (zone->type == ZoneType::BATTLEFIELD){
                updateZone(layout.battlefield, zone);
            }
            else if (zone->type == ZoneType::GRAVEYARD){
                Card* card = zone->drawTop();
                if (!card){
                    continue;
                }
                QPixmap image = QPixmap::fromImage(card->image);
                layout.graveyard->setPixmap(image.scaled(layout.graveyard->size()));
            }
            else if (zone->type == ZoneType::EXILE){
                Card* card = zone->drawTop();
                if (!card){
                    continue;
                }
                QPixmap image = QPixmap::fromImage(card->image);
                layout.exile->setPixmap(image.scaled(layout.exile->size()));
            }
        }

        // Set the Mana
        for (auto [color, amount] : currPlayer->manaPool.toStdMap()){
            switch (color) {
            // case ManaType::RED:   layout.red->setText(QString::number(amount));
            case ManaType::BLUE:  layout.blue->setText(QString::number(amount));
            case ManaType::GREEN: layout.green->setText(QString::number(amount));
            case ManaType::BLACK: layout.black->setText(QString::number(amount));
            case ManaType::WHITE: layout.white->setText(QString::number(amount));
            default:              break;
            }
        }

        // Set the Health
        layout.health->setText(QString::number(currPlayer->health));

        update();
    }
}

void MainWindow::updateZone(QGridLayout* container, Zone* zone){

    if (container == nullptr){
        return;
    }
    // Clear Zone
    QLayoutItem* item;
    while((item = container->takeAt(0)) != nullptr){
        if (item->widget()){
            delete item->widget();
        }
        delete item;
    }

    for(Card* card : *zone){
        CardButton* cardButton = new CardButton(card);
        activeCards.append(cardButton);

        connect(cardButton, &CardButton::cardSelected, this, &MainWindow::handleCardSelected);
        connect(cardButton, &CardButton::hovered, this, &MainWindow::updateMagnifier);
        cardButton->setFixedSize(100, 140);

        if(card->type == CardType::LAND && zone->type == ZoneType::BATTLEFIELD){
            // landGroups[card->color].append(cardButton);
            ui->playerRed->setPixmap(QPixmap::fromImage(card->image).scaled(ui->playerRed->size()));
            currentSelectedCard = nullptr;
            continue;
        }

        container->addWidget(cardButton, 0, container->count(), Qt::AlignCenter);

    }
    update();
}


void MainWindow::clearSelection(){

    selectedButtons.clear();
    currentSelectedCard = nullptr;

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


