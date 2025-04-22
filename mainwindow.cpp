#include "mainwindow.h"
#include "cardapimanager.h"
#include "ui_mainwindow.h"
#include "carddictionary.h"
#include "gamemanager.h"
#include "textparser.h"
#include "phase.h"
#include <QTimer>
#include <QDebug>
#include <QtGui/qevent.h>
#include "ManaCollection.h"
#include <QMessageBox>
#include <type.h>
#include <QScrollArea>
#include <QIcon>

MainWindow::MainWindow(gamemanager* game, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Magic Tutorial");

    ui->CardDescription->setReadOnly(true);
    ui->CardDescription->setFocusPolicy(Qt::NoFocus);

    apiManager = new CardAPIManager(this);
    statePointer = game->state;
    userPlayer = statePointer->player1;
    userPlayer->holdingPriority = true;
    QMap<ManaType, int>* mana = new QMap<ManaType, int>;
    (*mana)[ManaType::RED] = 1;
    userPlayer->addMana(mana);
    userPlayer->isActivePlayer = true;
    enemyPlayer = statePointer->player2;
    enemyPlayer->isActivePlayer = false;
    enemyPlayer->holdingPriority = false;

    playerLayout = {
        ui->playerHand,
        ui->playerBattlefield,
        ui->playerGraveyardButton,
        "Graveyard",
        ui->playerExileButton,
        "Exile",
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
        nullptr,
        "EnemyGraveyard",
        nullptr,
        "EnemyExile",
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

    //Connect Exile and Graveyard
    // connect(ui->playerGraveyard, &QPushButton::clicked, this, [=]() {
    //     showZoneDialog(&graveyardButtons, "Graveyard");
    //     updateUI();
    // });

    // connect(ui->playerExile, &QPushButton::clicked, this, [=]() {
    //     showZoneDialog(&exileButtons, "Exile");
    //     updateUI();
    // });

    //update Icon for zones
    // ui->playerExile->setFixedSize(100,70);
    // ui->playerGraveyard->setFixedSize(100,70);
    ui->playerDeck->setFixedSize(100,140);
    ui->playerDeck->setPixmap(QPixmap(":/Icons/Icons/BackCard.png").scaled(ui->playerDeck->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //set up for mana
    // int landCount = landGroups[ManaType::RED].size();
    // ui->playerRedIcon->setText(QString(" %1").arg(landCount));
    ui->playerRedIcon->setText("0");
    ui->playerRedIcon->setIcon(QIcon(":/Icons/Icons/Red.png"));

    ui->playerBlackIcon->setText("0");
    ui->playerBlackIcon->setIcon(QIcon(":/Icons/Icons/Black.png"));

    ui->playerWhiteIcon->setText("0");
    ui->playerWhiteIcon->setIcon(QIcon(":/Icons/Icons/white.png"));

    ui->playerGreenIcon->setText("0");
    ui->playerGreenIcon->setIcon(QIcon(":/Icons/Icons/Green.png"));

    ui->playerBlueIcon->setText("0");
    ui->playerBlueIcon->setIcon(QIcon(":/Icons/Icons/Blue.png"));

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

    connect(ui->playerGraveyardButton, &QPushButton::clicked, this, [=]() {
        showCollection("Graveyard");
    });

    connect(ui->playerExileButton, &QPushButton::clicked, this, [=]() {
        showCollection("Exile");
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

    // connect(ui->phaseButton, &QPushButton::clicked, this, [=](){
    //     // TESTING, WILL REMOVE
    //     qDebug() << "Next Phase!";
    //     if (!statePointer) {
    //         qWarning() << "State pointer is null!";
    //         return;
    //     }

    //     userPlayer->hasPlayedLand = false;

    //     statePointer->changePhase();
    //     //statePointer->changeActivePlayer();
    //     clearSelection();
    //     if(statePointer->currentPhase == Phase::PreCombatMain){
    //         qDebug() << "Current Phase: PreCombat";
    //     }
    //     else if (statePointer->currentPhase == Phase::DeclareAttackers){
    //         qDebug() << "Current Phase: Attacking";
    //     }
    //     else if (statePointer->currentPhase == Phase::DeclareBlockers){
    //         qDebug() << "Current Phase: Defending";
    //     }
    //     else if(statePointer->currentPhase == Phase::PostCombatMain){
    //         qDebug() << "Current Phase: PostCombat";
    //     }
    //     updateUI();
    // });

    connect(ui->phaseButton, &QPushButton::clicked, game, &gamemanager::onChangePhase);
    connect(ui->priorityButton, &QPushButton::clicked, game, &gamemanager::onPassPriority);

    connect(game, &gamemanager::updateUI, this, &MainWindow::updateUI);

    // TESTING
    connect(ui->targetButton, &QPushButton::clicked, this, &MainWindow::startTargeting);

    for(QString cardName : TextParser::getListFromText(QFile(":/text/additional_files/deck.txt"))){
        qDebug() << cardName;
        apiManager->fetchCardByName(cardName);
    }

    QTimer::singleShot(2000, this, [=](){
        qDebug() << "SETUP CALLED";
        setupHand();
    });

    connect(this, &MainWindow::sendCombatCards, game, &gamemanager::onCombatCardsReceived);

}

MainWindow::~MainWindow() {
    delete ui;
}

//FOR TESTING
void MainWindow::setupHand(){

    card1 = cardDictionary::getCard("Lightning Bolt");
    qDebug() << "CARD NAME: " << card1.name;

    card2 = cardDictionary::getCard("Coral Merfolk");
    card3 = cardDictionary::getCard("Goblin Bully");
    card4 = cardDictionary::getCard("Shock");
    card5 = cardDictionary::getCard("Shock");
    card6 = cardDictionary::getCard("Counterspell");
    card7 = cardDictionary::getCard("Mountain");
    card8 = cardDictionary::getCard("Hill Giant");
    card9 = cardDictionary::getCard("Canyon Minotaur");
    card10 = cardDictionary::getCard("Bonebreaker Giant");

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

    enemyPlayer->Battlefield.addCard(&card8, false);
    enemyPlayer->Battlefield.addCard(&card9, false);
    enemyPlayer->Battlefield.addCard(&card10, false);



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

// Need to add more logic
void MainWindow::cardBeingTapped(CardButton* cardButton, bool tapped){
    // check with game state to allow the card to be tapped or untapped
    //Now just hard coded for testing
    if(currentSelectedCard != cardButton){
        handleCardSelected(cardButton);
    }
    qDebug() << currentSelectedCard->cardName;
    currentSelectedCard->setTapped(!tapped);

    // add logic if card is a land
    // add logic if card is a creature
}

void MainWindow::updateManaButton(ManaType type) {
    int count = landGroups[type].size();
    QPushButton* button = nullptr;

    switch (type) {
        case ManaType::WHITE: button = ui->playerWhiteIcon; break;
        case ManaType::BLUE:  button = ui->playerBlueIcon; break;
        case ManaType::RED:  button = ui->playerRedIcon; break;
        case ManaType::BLACK:  button = ui->playerBlackIcon; break;
        case ManaType::GREEN:  button = ui->playerGreenIcon; break;
        default: break;
    }

    if (button) {
        button->setText(QString(" %1").arg(count));
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
        qDebug() << land->tapped;
        layout->addWidget(land);
        if(land->isEnabled()){
            land->enableCard(true);
        }
        else{
            land->enableCard(false);
        }
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

bool MainWindow::promptForMana(){
    ManaCollection* manaPrompt = new ManaCollection(userPlayer, currentSelectedCard->cardPtr, this);

    if(manaPrompt->exec() == QDialog::Accepted){
        userPlayer->selectedMana = manaPrompt->getUserMana();
        return true;
    }
    else{
        return false;
    }
}
// void MainWindow::showZoneDialog(QVector<CardButton*>* zoneCards, const QString& title) {
//     if (!zoneCards) return;

//     QDialog* dialog = new QDialog(this);
//     dialog->setWindowTitle(title);
//     dialog->setMinimumSize(600, 250);

//     QScrollArea* scrollArea = new QScrollArea(dialog);
//     scrollArea->setWidgetResizable(true);

//     QWidget* container = new QWidget;
//     QHBoxLayout* layout = new QHBoxLayout(container);


//     qDebug() << zoneCards->size();
//     for (CardButton* cardButton : *zoneCards) {
//         qDebug() << "Enter loop";
//         cardButton->setParent(container);   // move it into the dialog
//         cardButton->setVisible(true);       // ensure it's shown
//         layout->addWidget(cardButton);      // attach to layout
//     }

//     scrollArea->setWidget(container);

//     QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
//     mainLayout->addWidget(scrollArea);
//     dialog->setLayout(mainLayout);

//     dialog->exec();  // modal popup
// }

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
        // TODO: Hard coded need to change
        landGroups[ManaType::RED].append(currentSelectedCard);

        currentSelectedCard->setChecked(false);
        currentSelectedCard = nullptr;

        qDebug() << "Played land to mana stack.";
        qDebug() << ui->playerHand->count();

        // Hard coded need to change
        updateManaButton(ManaType::RED);

        userPlayer->hasPlayedLand = true;
    }

    else if (card->isPermanent){
        // FOR TESTING PURPOSES
        userPlayer->moveCardZone(card, userPlayer->Hand, userPlayer->Battlefield, false);

        currentSelectedCard->setChecked(false);
        currentSelectedCard = nullptr;

        qDebug() << "Played creature to battlefield.";
        clearSelection();
    }
    else {
        userPlayer->moveCardString(card, "hand", "graveyard", true);
        clearSelection();
    }
    updateUI();
}

void MainWindow::handleCardSelected(CardButton* clicked) {
    if(statePointer->currentPhase == Phase::DeclareAttackers ||
        statePointer->currentPhase == Phase::DeclareBlockers){
        if(selectedButtons.contains(clicked)){
            selectedButtons.removeOne(clicked);
            clicked->setChecked(false);
            currentSelectedCard = nullptr;
            qDebug() << "Removed " << clicked->cardPtr->name;
        }
        else{
            selectedButtons.append(clicked);
            clicked->setChecked(true);
            currentSelectedCard = clicked;
            qDebug() << "Added " << clicked->cardPtr->name;
        }
        overlayCards();
    }
    else{
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

    activeCards.clear();

    qDebug() << "updateUI called";
    QVector<Zone*> zones;
    ZoneLayout layout;
    Player* currPlayer;

    if(!userPlayer){
        qDebug() << "NO USER";
        return;
    }

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
                updateDeck(zone, layout.graveName, layout.graveyard);
            }
            else if (zone->type == ZoneType::EXILE){
                updateDeck(zone, layout.exileName, layout.exile);
            }
        }

        // update Stack zone
        //need to implement stackZone
        // updateZone(ui->stack, //zone);

        qDebug() << "update Mana";
        // Set the Mana
        for (auto [color, amount] : currPlayer->manaPool.toStdMap()){
            switch (color) {
            case ManaType::RED:   layout.red->setText(QString("Red Mana: ") + QString::number(amount)); break;
            case ManaType::BLUE:  layout.blue->setText(QString("Blue Mana: ") + QString::number(amount)); break;
            case ManaType::GREEN: layout.green->setText(QString("Green Mana: ") + QString::number(amount)); break;
            case ManaType::BLACK: layout.black->setText(QString("Black Mana: ") + QString::number(amount)); break;
            case ManaType::WHITE: layout.white->setText(QString("White Mana: ") + QString::number(amount)); break;
            default:              break;
            }
        }

        qDebug() << "update Health";

        // Set the Health
        layout.health->setText(QString::number(currPlayer->health));

        qDebug() << "update Phases";
        handlePhase();

        update();
        qDebug() << "updateUI has finished";
    }
}

void MainWindow::updateZone(QGridLayout* container, Zone* zone){
    qDebug() << "Updating Zone";

    if (container == nullptr){
        return;
    }



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

    // should we another container and another Stack zone so we can move cardButton from player hand to the stack before hitting the field?

    for(Card* card : *zone){
        CardButton* cardButton = new CardButton(card);
        activeCards.append(cardButton);

        connect(cardButton, &CardButton::cardSelected, this, &MainWindow::handleCardSelected);
        connect(cardButton, &CardButton::hovered, this, &MainWindow::updateMagnifier);
        connect(cardButton, &CardButton::cardTapped, this, &MainWindow::cardBeingTapped);

        cardButton->setFixedSize(100, 140);

        if(card->type == CardType::LAND && zone->type == ZoneType::BATTLEFIELD){
            landGroups[card->color].append(cardButton);
            currentSelectedCard = nullptr;
            continue;
        }


        //Test
        if(zone->type != ZoneType::GRAVEYARD && zone->type != ZoneType::EXILE){
            container->addWidget(cardButton, 0, container->count(), Qt::AlignCenter);
        }

        // container->addWidget(cardButton, 0, container->count(), Qt::AlignCenter);

    }
    // update();
}

void MainWindow::updateDeck(Zone* zone, QString title, QPushButton *deckButton){
    if(!zone){
        qDebug() << "Zone Not Found";
        return;
    }

    containerCards[title].clear();

    for(Card* card : *zone){
        CardButton* button = new CardButton(card);
        activeCards.append(button);
        containerCards[title].prepend(button);

        connect(button, &CardButton::cardSelected, this, &MainWindow::handleCardSelected);
        connect(button, &CardButton::hovered, this, &MainWindow::updateMagnifier);
        connect(button, &CardButton::cardTapped, this, &MainWindow::cardBeingTapped);
        button->setFixedSize(100, 140);

        qDebug() << "Added Card: " << card->name << "Count: " << containerCards[title].count();
        qDebug() << "Card in graveyard: " << containerCards["Graveyard"].count() << "Cards in Exile: " << containerCards["Exile"].count();
    }

    if(!deckButton){
        return;
    }

    Card* card = zone->drawTop();

    if(!card){
        deckButton->setIcon(QIcon());
        if (zone->type == ZoneType::GRAVEYARD){
            deckButton->setText("Graveyard");
        }
        else{
            deckButton->setText("Exile");
        }
        return;
    }
    deckButton->setText("");
    QPixmap image = QPixmap::fromImage(card->image.scaled(deckButton->size() - QSize(10,10), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    deckButton->setIcon(QIcon(image));
    deckButton->setIconSize(deckButton->size());
}

void MainWindow::showCollection(QString title){

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(title);
    dialog->setMinimumSize(400, 300);

    QScrollArea* scrollArea = new QScrollArea(dialog);
    QWidget* container = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(container);

    qDebug() << "Collection size: " << containerCards[title].size();

    for (CardButton* card : containerCards[title]) {
        layout->addWidget(card);
        if(card->isEnabled()){
            card->enableCard(true);
        }
        else{
            card->enableCard(false);
        }
        qDebug() << "added";
    }

    container->setLayout(layout);
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
    mainLayout->addWidget(scrollArea);
    dialog->setLayout(mainLayout);

    dialog->exec();
}


void MainWindow::clearSelection(){
    qDebug() << "clearing Selection";

    selectedButtons.clear();
    currentSelectedCard = nullptr;

    for(CardButton* card : activeCards){
        card->setChecked(false);
    }
    update();

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
    statePointer->getValidActions();

    // PhaseRules rules = statePointer->getPhaseRules();
    // bool isActive = userPlayer->isActivePlayer;

    for (CardButton* button : activeCards){
        if (!button) {
            qDebug() << "Warning: Null button found in activeCards";
            continue;
        }
        Card* card = button->cardPtr;
        // bool shouldEnable = false;

        // if(rules.canPlayInstant && card->type == CardType::INSTANT){
        //     shouldEnable = true;
        // }
        // if(rules.canPlaySorcery && card->type == CardType::SORCERY){
        //     shouldEnable = true;
        // }
        // if(isActive && rules.canDeclareAttack && card->type == CardType::CREATURE){
        //     shouldEnable = true;
        // }
        // if(!isActive && rules.canDeclareDefense && card->type == CardType::CREATURE){
        //     shouldEnable = true;
        // }
        // if(isActive && card->type == CardType::LAND && !userPlayer->hasPlayedLand){
        //     shouldEnable = true;
        // }
        // if(isActive && rules.canPlaySorcery &&
        //         (card->type == CardType::CREATURE ||
        //          card->type == CardType::ARTIFACT ||
        //          card->type == CardType::ENCHANTMENT ||
        //          card->type == CardType::PLANESWALKER ||
        //          card->type == CardType::BATTLE)) {
        //     shouldEnable = true;
        // }
        button->enableCard(card->shouldEnable);
        ui->priorityButton->setEnabled(statePointer->player1->canChangePhase);
        ui->phaseButton->setEnabled(statePointer->player1->canPassPriority);
        // update();
    }
}

void MainWindow::startTargeting(){
    isTargeting = true;
    // IMPLEMENT ISTARGETING IN CARD SELECTION
    selectedCards.clear();

    for(CardButton* button : activeCards){
        button->enableCard(false);
    }

    QGridLayout* targetZone = ui->enemyBattlefield;

    for(int i = 0; i < targetZone->count(); i++){

        QLayoutItem* item = targetZone->itemAt(i);
        QWidget* widget = item->widget();
        CardButton* button = qobject_cast<CardButton*>(widget);
        button->enableCard(true);
    }
}

void MainWindow::overlayCards(){
    if(!activeCards.isEmpty()){
        for(CardButton* button : activeCards){
            button->resetCard();
        }
    }

    for(int i = 0; i < selectedButtons.count(); i++){
        CardButton* currButton = selectedButtons.at(i);
        QPixmap newIcon = currButton->getOverlayedPixmap(i);
        currButton->setIcon(QIcon(newIcon));
    }
}
