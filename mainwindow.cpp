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
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

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
        ui->playerRedIcon,
        ui->playerGreenIcon,
        ui->playerBlueIcon,
        ui->playerWhiteIcon,
        ui->playerBlackIcon,
        ui->PlayerHealth,
        ui->phaseLabel,
        &playerLandGroups
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
        ui->enemyRedIcon,
        ui->enemyGreenIcon,
        ui->enemyBlueIcon,
        ui->enemyWhiteIcon,
        ui->enemyBlackIcon,
        ui->EnemyHealth,
        ui->phaseLabel,
        &enemyLandGroups
    };


    connect(apiManager, &CardAPIManager::errorOccurred, this, [](const QString &error) {
        qDebug() << "API Error:" << error;
    });

    connect(ui->playCardButton, &QPushButton::clicked, this, &MainWindow::onPlayCardButtonClicked);

    //update Icon for zones
    ui->playerDeck->setFixedSize(100,140);
    ui->playerDeck->setPixmap(QPixmap(":/Icons/Icons/BackCard.png").scaled(ui->playerDeck->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //set up for mana

    ui->playerRedIcon->setText("0");
    ui->playerRedIcon->setIcon(QIcon(":/Icons/Icons/Red.png"));
    ui->enemyRedIcon->setIcon(QIcon(":/Icons/Icons/Red.png"));

    ui->playerBlackIcon->setText("0");
    ui->playerBlackIcon->setIcon(QIcon(":/Icons/Icons/Black.png"));
    ui->enemyBlackIcon->setIcon(QIcon(":/Icons/Icons/Black.png"));


    ui->playerWhiteIcon->setText("0");
    ui->playerWhiteIcon->setIcon(QIcon(":/Icons/Icons/white.png"));
    ui->enemyWhiteIcon->setIcon(QIcon(":/Icons/Icons/white.png"));

    ui->playerGreenIcon->setText("0");
    ui->playerGreenIcon->setIcon(QIcon(":/Icons/Icons/Green.png"));
    ui->enemyGreenIcon->setIcon(QIcon(":/Icons/Icons/Green.png"));

    ui->playerBlueIcon->setText("0");
    ui->playerBlueIcon->setIcon(QIcon(":/Icons/Icons/Blue.png"));
    ui->enemyBlueIcon->setIcon(QIcon(":/Icons/Icons/Blue.png"));

    connect(ui->playerRedIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::RED, playerLayout);
    });

    connect(ui->playerWhiteIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::WHITE, playerLayout);
    });

    connect(ui->playerGreenIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::GREEN, playerLayout);
    });

    connect(ui->playerBlueIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::BLUE, playerLayout);
    });

    connect(ui->playerBlackIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::BLACK, playerLayout);
    });

    connect(ui->playerGraveyardButton, &QPushButton::clicked, this, [=]() {
        showCollection("Graveyard");
    });

    connect(ui->playerExileButton, &QPushButton::clicked, this, [=]() {
        showCollection("Exile");
    });

    connect(ui->enemyRedIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::RED, enemyLayout);
    });

    connect(ui->enemyWhiteIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::WHITE, enemyLayout);
    });

    connect(ui->enemyGreenIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::GREEN, enemyLayout);
    });

    connect(ui->enemyBlueIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::BLUE, enemyLayout);
    });

    connect(ui->enemyBlackIcon, &QPushButton::clicked, this, [=]() {
        showLandPopup(ManaType::BLACK, enemyLayout);
    });



    connect(ui->phaseButton, &QPushButton::clicked, game, &gamemanager::onChangePhase);
    connect(ui->priorityButton, &QPushButton::clicked, game, &gamemanager::onPassPriority);

    connect(game, &gamemanager::updateUI, this, &MainWindow::updateUI);

    // TESTING
    connect(ui->tapButton, &QPushButton::clicked, this, [=]() {
        cardBeingTapped(currentSelectedCard, currentSelectedCard->tapped);
    });

    QTimer::singleShot(200, this, [=](){
        qDebug() << "SETUP CALLED";
        setupHand();
    });

    connect(this, &MainWindow::playCard, game, &gamemanager::onPlayCard);

    connect(this, &MainWindow::sendCombatCards, game, &gamemanager::onCombatCardsReceived);
}

MainWindow::~MainWindow() {
    delete ui;
}

//FOR TESTING
void MainWindow::setupHand(){

    if(userPlayer){
        qDebug() << "Found User!";
    }
    else {
        return;
    }
    updateUI();
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

QString MainWindow::cardTypeToString(CardType type) {
    switch (type) {
    case CardType::LAND:    return "Land";
    case CardType::CREATURE:  return "Creature";
    case CardType::ARTIFACT:  return "Artifact";
    case CardType::ENCHANTMENT:  return "Enchantment";
    case CardType::PLANESWALKER:  return "Planeswalker";
    case CardType::BATTLE:  return "Battle";
    case CardType::INSTANT:  return "Instant";
    case CardType::SORCERY:        return "Sorcery";
    default:              return "Unknown";
    }
}

QString MainWindow::phaseTypeToString(Phase phase) {
    switch (phase) {
    case Phase::Untap:   return "Untap";
    case Phase::Upkeep:   return "Upkeep";
    case Phase::Draw:   return "Draw";
    case Phase::PreCombatMain:   return "PreCombatMain";
    case Phase::BeginCombat:   return "BeginCombat";
    case Phase::DeclareAttackers:   return "DeclareAttackers";
    case Phase::DeclareBlockers:   return "DeclareBlockers";
    case Phase::CombatDamage:   return "CombatDamage";
    case Phase::PostCombatMain:   return "PostCombatMain";
    case Phase::EndStep:   return "EndStep";
    case Phase::Cleanup:   return "Cleanup";
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

    if(!tapped){
        emit tapCard(currentSelectedCard->cardPtr);
    }

    currentSelectedCard->setTapped(!tapped);

    // add logic if card is a land
    // add logic if card is a creature
}

void MainWindow::updateManaButton(ManaType type, ZoneLayout layout) {
    int count = layout.landGroups->value(type).size();
    QPushButton* button = nullptr;

    switch (type) {
        case ManaType::WHITE:  button = layout.whiteCount; break;
        case ManaType::BLUE:   button = layout.blueCount; break;
        case ManaType::RED:    button = layout.redCount; break;
        case ManaType::BLACK:  button = layout.blackCount; break;
        case ManaType::GREEN:  button = layout.greenCount; break;
        default: break;
    }

    if (button) {
        button->setText(QString(" %1").arg(count));
    }
}

void MainWindow::showLandPopup(ManaType manaType, ZoneLayout zoneLayout){
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Lands: " + manaTypeToString(manaType));
    dialog->setMinimumSize(400, 300);

    QScrollArea* scrollArea = new QScrollArea(dialog);
    QWidget* container = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(container);

    qDebug() << "Mana stack size:" << zoneLayout.landGroups->value(manaType).size();
    qDebug() << manaTypeToString(manaType);

    for (CardButton* land : zoneLayout.landGroups->value(manaType)) {
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

void MainWindow::onPlayCardButtonClicked(){
    // Just for testing need to handle more condiction phrases, cost, priority
    if (!currentSelectedCard){
        QMessageBox::information(this, "No Card Selected", "Select a card to play.");
        return;
    }

    if(isTargeting){
        emit playCard(targetSource, currentSelectedCard->cardPtr);
    }

    emit playCard(currentSelectedCard->cardPtr, nullptr);

    clearSelection();
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
    QString info;
    info += "<b>Name:</b> " + cardFromDictionary.name + "<br>";
    info += "<b>Type:</b> " + cardTypeToString(cardFromDictionary.type) + "<br>";

    QString costLine;
    for (auto it = cardFromDictionary.cost.begin(); it != cardFromDictionary.cost.end(); ++it) {
        if (it.value() > 0) {
            costLine += QString("%1 %2, ")
            .arg(it.value())
                .arg(MainWindow::manaTypeToString(it.key()));
        }
    }
    if (!costLine.isEmpty()) {
        costLine.chop(2);
        info += "<b>Cost:</b> " + costLine + "<br>";
    } else {
        info += "<b>Cost:</b> 0<br>";
    }

    if (cardFromDictionary.type == CardType::CREATURE) {
        info += QString("<b>Power/Toughness:</b> %1/%2<br>")
        .arg(cardFromDictionary.power)
            .arg(cardFromDictionary.toughness);
    }

    info += "<hr>";
    info += "<b>Ability:</b><br>" + cardFromDictionary.description;

    qDebug() << cardFromDictionary.flavorText;
    info += "<hr>";
    if (!cardFromDictionary.flavorText.isEmpty()) {
        info += "<br><i style='color:gray'>" + cardFromDictionary.flavorText + "</i>";
    }

    // Show formatted description in the magnifier
    ui->CardDescription->setText(info);
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

        layout.landGroups->clear();

        // Go through all zones and update containers
        for (Zone* zone : zones){
            if (zone->type == ZoneType::HAND){
                updateZone(layout.hand, zone, layout.landGroups);
            }
            else if (zone->type == ZoneType::BATTLEFIELD){
                updateZone(layout.battlefield, zone, layout.landGroups);
            }
            else if (zone->type == ZoneType::GRAVEYARD){
                updateDeck(zone, layout.graveName, layout.graveyard);
            }
            else if (zone->type == ZoneType::EXILE){
                updateDeck(zone, layout.exileName, layout.exile);
            }
        }

        // update Stack zone


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

        for (ManaType color : layout.landGroups->keys()){
            updateManaButton(color, layout);
        }

        qDebug() << "update Health";

        // Set the Health
        layout.health->setText(QString::number(currPlayer->health));

        // QString("Phase: ") +
        layout = playerLayout;
        layout.phaseLabel->setText(QString("Phase: ") + phaseTypeToString(statePointer->currentPhase));

        qDebug() << "update Phases";
        handlePhase();

        update();
        qDebug() << "updateUI has finished";
    }

    //update Stack
    QGridLayout* container = ui->stack;

    // Clear current stack view
    QLayoutItem* item;
    while ((item = container->takeAt(0)) != nullptr) {
        if (item->widget()){
            delete item->widget();
        }
        delete item;            // delete the layout wrapper
    }

    // Re-add current stack contents
    for (const StackObject &object : statePointer->theStack) {
        qDebug() << "accessing the stack";

        CardButton* cardButton = new CardButton(object.card);
        activeCards.append(cardButton);

        connect(cardButton, &CardButton::cardSelected, this, &MainWindow::handleCardSelected);
        connect(cardButton, &CardButton::hovered, this, &MainWindow::updateMagnifier);
        connect(cardButton, &CardButton::cardTapped, this, &MainWindow::cardBeingTapped);
        cardButton->setFixedSize(100, 140);
        container->addWidget(cardButton);
    }
}

void MainWindow::updateZone(QGridLayout* container, Zone* zone, QMap<ManaType, QList<CardButton *>>* landGroups){
    qDebug() << "Updating Zone";
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
        // QSize targetSize = card->isTapped ? QSize(140, 100) : QSize(100, 140);
        cardButton->setFixedSize(100, 140);


        if(card->type == CardType::LAND && zone->type == ZoneType::BATTLEFIELD){
            (*landGroups)[card->color].append(cardButton);
            currentSelectedCard = nullptr;
            continue;
        }


        //Test
        // if(zone->type != ZoneType::GRAVEYARD && zone->type != ZoneType::EXILE){
        //     container->addWidget(cardButton, 0, container->count(), Qt::AlignCenter);
        // }

        container->addWidget(cardButton, 0, container->count(), Qt::AlignCenter);

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

    dialog->show();
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

void MainWindow::startTargeting(Card *sourceCard){
    isTargeting = true;
    targetSource = sourceCard;
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

void MainWindow::onGameEnded(bool playerWon) {
    endWorld = new b2World(b2Vec2(0, 0));  // no gravity

    endScene = new QGraphicsScene(this);
    endView = new QGraphicsView(endScene, this);
    endView->setGeometry(this->rect());
    endView->setStyleSheet("background: transparent;");
    // endView->setFrameShape(QFrame::NoFrame);
    // endView->setSceneRect(0, 0, width(), height());
    // endView->setAttribute(Qt::WA_TransparentForMouseEvents);
    endView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    endView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    endView->show();

    // 3. Spawn cards from the center and push outward
    for (int i = 0; i < 30; ++i) {
        QPixmap pix(":/Icons/Icons/BackCard.png");
        QGraphicsPixmapItem* card = endScene->addPixmap(pix.scaled(60, 90));
        card->setOffset(-30, -45);  // center origin
        card->setPos(width() / 2, height() / 2);

        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(width() / 2, height() / 2);
        def.angle = (rand() % 360) * b2_pi / 180.0f;

        b2Body* body = endWorld->CreateBody(&def);

        b2PolygonShape shape;
        shape.SetAsBox(30, 45);

        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.density = 1.0f;
        fixture.restitution = 0.8f;
        body->CreateFixture(&fixture);

        //Explosion
        float x = (rand());
        float y = (rand());
        body->ApplyLinearImpulse(b2Vec2(x, y), body->GetWorldCenter(), true);

        endfallingCards.append(qMakePair(card, body));
    }

    // Message
    QLabel* label = new QLabel(this);
    label->setText(playerWon ? "🎉 You Win! 🎉" : "😞 You Lose 😞");
    label->setStyleSheet("color: white; background-color: rgba(0,0,0,180); font-size: 28px; padding: 20px; border-radius: 10px;");
    label->setAlignment(Qt::AlignCenter);
    label->setGeometry((width() - 300) / 2, height() / 2 - 60, 300, 80);
    label->raise();
    label->show();


    endTimer = new QTimer(this);
    connect(endTimer, &QTimer::timeout, this, &MainWindow::updateEndExplosion);
    endTimer->start(20);
}

void MainWindow::updateEndExplosion() {
    endWorld->Step(1.0 / 30.0, 3, 1);

    for (auto& pair : endfallingCards) {
        QGraphicsPixmapItem* card = pair.first;
        b2Body* body = pair.second;

        b2Vec2 pos = body->GetPosition();
        float angle = body->GetAngle();

        card->setPos(pos.x, pos.y);
        card->setRotation(angle * 180 / b2_pi);
    }
}

