#include "mainwindow.h"
#include "cardapimanager.h"
#include "lotusdialog.h"
#include "ui_mainwindow.h"
#include "carddictionary.h"
#include "gamemanager.h"
#include "phase.h"
#include "bot.h"
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
#include <QFontDatabase>
#include <QPixmap>
#include "settings.h"
#include "mainmenu.h"

MainWindow::MainWindow(gamemanager* game, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Magic Tutorial");

    this->setStyleSheet("QMainWindow {background-image: url(:/Icons/Icons/backgroundv4.png); }");
    this->setFixedSize(this->size());

    ui->CardDescription->setReadOnly(true);
    ui->CardDescription->setFocusPolicy(Qt::NoFocus);

    apiManager = new CardAPIManager(this);
    statePointer = game->state;
    userPlayer = statePointer->player1;
    isTargeting = false;
    userPlayer->holdingPriority = true;
    userPlayer->isActivePlayer = true;
    enemyPlayer = statePointer->player2;
    enemyPlayer->isActivePlayer = false;
    enemyPlayer->holdingPriority = false;

    ui->enemyTargetButton->setVisible(false);

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
        ui->activePlayerLabel,
        &playerLandGroups
    };

    enemyLayout = {
        ui->enemyHand,
        ui->enemyBattlefield,
        ui->enemyGraveyardButton,
        "EnemyGraveyard",
        ui->enemyExileButton,
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
        ui->activePlayerLabel,
        &enemyLandGroups
    };

    connect(ui->winButton, &QPushButton::clicked,this, [=]() {
        onGameEnded(true);
    });

    connect(apiManager, &CardAPIManager::errorOccurred, this, [](const QString &error) {
    });

    connect(ui->playCardButton, &QPushButton::clicked, this, &MainWindow::onPlayCardButtonClicked);

    ui->EnemyHealthIcon->setGeometry(ui->playerHealthIcon->geometry());
    ui->EnemyHealthIcon->setPixmap(QPixmap(":/Icons/Icons/hp.jpg"));
    ui->EnemyHealthIcon->setScaledContents(true);
    ui->playerHealthIcon->setPixmap(QPixmap(":/Icons/Icons/hp.jpg"));
    ui->playerHealthIcon->setScaledContents(true);
    // ui->playerDeck->setFixedSize(100,140);
    ui->playerDeck->setPixmap(QPixmap(":/Icons/Icons/BackCard.png").scaled(ui->playerDeck->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ui->enemyDeck->setFixedSize(100,140);
    ui->enemyDeck->setPixmap(QPixmap(":/Icons/Icons/BackCard.png").scaled(ui->playerDeck->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));


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

    connect(ui->enemyGraveyardButton, &QPushButton::clicked, this, [=]() {
        showCollection("EnemyGraveyard");
    });

    connect(ui->enemyExileButton, &QPushButton::clicked, this, [=]() {
        showCollection("EnemyExile");
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

    connect(ui->enemyTargetButton, &QPushButton::clicked, this, [=]() {
        emit playCard(targetSource, enemyPlayer);
        stopTargeting();
    });

    connect(game, &gamemanager::gameOver, this, &MainWindow::onGameEnded);

    connect(ui->phaseButton, &QPushButton::clicked, game, &gamemanager::onChangePhase);
    connect(ui->priorityButton, &QPushButton::clicked, game, &gamemanager::onPassPriority);

    connect(this, &MainWindow::displayGameTip, game, &gamemanager::displayPhaseTip);

    connect(game, &gamemanager::updateUI, this, &MainWindow::updateUI);

    connect(game, &gamemanager::requestMainWindowPos, this, &MainWindow::getMainWindowPos);
    connect(this, &MainWindow::updateMainWindowPos, game, &gamemanager::onReceiveMainWindowPos);

    connect(game, &gamemanager::promptTargeting, this, &MainWindow::startTargeting);

    // TESTING
    connect(ui->tapButton, &QPushButton::clicked, this, &MainWindow::cardBeingTapped);
    connect(this, &MainWindow::tapCard, game, &gamemanager::onTapCard);

    QTimer::singleShot(100, this, [=](){
        setupHand();
    });

    connect(this, &MainWindow::playCard, game, &gamemanager::onPlayCard);

    connect(this, &MainWindow::sendCombatCards, game, &gamemanager::onCombatCardsReceived);

    connect(game, &gamemanager::gameOver, this, &MainWindow::onGameEnded);

    Bot* botEnemy = static_cast<Bot*>(enemyPlayer);
    if (botEnemy) {
        connect(botEnemy, &Bot::UiDeclareCombatants, this, &MainWindow::botDeclareCombatants);
        connect(botEnemy, &Bot::UiDeclareAttackers, this, &MainWindow::botDeclareAttackers);
        connect(botEnemy, &Bot::showBlockers, this, &MainWindow::displayBlockers);
    } else {

    }

    connect(ui->GameTipsCheckBox, &QCheckBox::toggled, game, &gamemanager::onToggleGameTips);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupHand(){

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
    default:               return "Unknown";
    }
}

QString MainWindow::phaseTypeToString(Phase phase) {
    switch (phase) {
    case Phase::Untap:   return "Untap";
    case Phase::Upkeep:   return "Upkeep";
    case Phase::Draw:   return "Draw";
    case Phase::PreCombatMain:   return "Pre-Combat Main";
    case Phase::BeginCombat:   return "Begin Combat";
    case Phase::DeclareAttackers:   return "Declare Attackers";
    case Phase::DeclareBlockers:   return "Declare Blockers";
    case Phase::CombatDamage:   return "Combat Damage";
    case Phase::PostCombatMain:   return "Post-Combat Main";
    case Phase::EndStep:   return "End Step";
    case Phase::Cleanup:   return "Cleanup";
    default:              return "Unknown";
    }
}

void MainWindow::cardBeingTapped(){
    if (!currentSelectedCard){
        QMessageBox::information(this, "No Card Selected", "Select a card to play.");
        return;
    }
    Card* card = currentSelectedCard->cardPtr;

    if(card->isTapped){
        QMessageBox::warning(this, "Card already tapped", "Card has already been tapped.");
        return;
    }

    if(card->name == "Black Lotus"){
        lotusHandling();
    }

    emit tapCard(card);
    clearSelection();
}

void MainWindow::cardTapped(CardButton* button){
    if(!button){
        QMessageBox::information(this, "No Card Selected", "Select a card to play.");
        return;
    }
    Card* card = button->cardPtr;

    if(card->isTapped){
        QMessageBox::warning(this, "Card already tapped", "Card has already been tapped.");
        return;
    }

    if(card->name == "Black Lotus"){
        lotusHandling();
        return;
    }

    emit tapCard(card);
    clearSelection();

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

    for (CardButton* land : zoneLayout.landGroups->value(manaType)) {
        // Create a horizontal layout for each land + button combination
        QWidget* landWidget = new QWidget();
        QHBoxLayout* landLayout = new QHBoxLayout(landWidget);

        // Add the land card to the layout
        landLayout->addWidget(land);

        // Create a tap button
        QPushButton* tapButton = new QPushButton("Tap Land");

        // Connect the tap button to a slot that will handle tapping the land
        connect(tapButton, &QPushButton::clicked, [=]() {
            // Toggle the tapped state
            if (land->cardPtr && !land->cardPtr->isTapped) {
                cardTapped(land);
                land->resetCard(); // Update the visual to reflect the new state
                dialog->repaint();
            }
            else {
                QMessageBox::information(this, "Land Tapped", "This card is already Tapped");
                update();
            }
        });

        landLayout->addWidget(tapButton);
        landWidget->setLayout(landLayout);

        // Add the combined widget to the main layout
        layout->addWidget(landWidget);
    }

    container->setLayout(layout);
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
    mainLayout->addWidget(scrollArea);

    QPushButton* closeButton = new QPushButton("Close");
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    mainLayout->addWidget(closeButton);

    dialog->setLayout(mainLayout);
    dialog->exec();  // modal

}

bool MainWindow::promptForMana(Card* card){
    ManaCollection* manaPrompt = new ManaCollection(userPlayer, card, this);

    if(manaPrompt->exec() == QDialog::Accepted){
        userPlayer->selectedMana = manaPrompt->getUserMana();
        return true;
    }
    else{
        return false;
    }
}

void MainWindow::getMainWindowPos(){
    emit updateMainWindowPos(static_cast<int>(this->geometry().left()),
                             static_cast<int>(this->geometry().right()),
                             static_cast<int>(this->geometry().top()),
                             static_cast<int>(this->geometry().bottom()));
}

void MainWindow::onPlayCardButtonClicked(){
    // Just for testing need to handle more condiction phrases, cost, priority
    if(statePointer->currentPhase == Phase::DeclareAttackers && userPlayer->isActivePlayer){
        collectAttackers();
        return;
    }

    if(statePointer->currentPhase == Phase::DeclareBlockers && !userPlayer->isActivePlayer){
        collectBlockers();
        clearSelection();
        return;
    }

    if (!currentSelectedCard){
        QMessageBox::information(this, "No Card Selected", "Select a card to play.");
        return;
    }
    Card* card = currentSelectedCard->cardPtr;

    if(isTargeting){
        emit playCard(targetSource, card);
        stopTargeting();
        return;
    }

    if(card->cost[ManaType::ANY] > 0){
        if(promptForMana(card)){
            if(isTargeting){
                emit playCard(targetSource, card);
                stopTargeting();
                return;
            }
            else {
                emit playCard(card, nullptr);
                return;
            }
        }
        else{
            QMessageBox::information(this, "Not Enough Mana", "Please Select Correct amount of Mana.");
            clearSelection();
            return;
        }
    }

    emit playCard(card, nullptr);

    clearSelection();
}

void MainWindow::handleCardSelected(CardButton* clicked) {
    if((statePointer->currentPhase == Phase::DeclareAttackers ||
        statePointer->currentPhase == Phase::DeclareBlockers) &&
        clicked->cardPtr->type == CardType::CREATURE){

        if(selectedButtons.contains(clicked)){
            selectedButtons.removeOne(clicked);
            clicked->setChecked(false);
            currentSelectedCard = nullptr;
        }
        else{
            selectedButtons.append(clicked);
            clicked->setChecked(true);
            currentSelectedCard = clicked;
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
    info += "<hr>";
    if (!cardFromDictionary.flavorText.isEmpty()) {
        info += "<br><i style='color:gray'>" + cardFromDictionary.flavorText + "</i>";
    }

    // Show formatted description in the magnifier
    ui->CardDescription->setText(info);
}

void MainWindow::collectAttackers(){
    combatants.clear();

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

        if(button->isChecked() && button->cardPtr->type == CardType::CREATURE){
            combatants[button->cardPtr];
        }
    }

    if(combatants.isEmpty()){
        statePointer->changePhase();
        extractCombatants();
        return;
    }

    targetIt = combatants.begin();

    clearSelection();

    statePointer->attackers = combatants.keys();
    statePointer->changePhase();

}

void MainWindow::botDeclareAttackers(QList<Card*> attackers){

    if(attackers.isEmpty()){
        extractCombatants();
        return;
    }

    // Clear Combatants
    combatants.clear();

    // Make Attackers Keys
    for(Card* card : attackers){
        combatants[card];
    }

    // Set iterator to the beginning of the attackers
    targetIt = combatants.begin();
}

void MainWindow::collectBlockers(){
    QList<Card*> blockers;

    if (targetIt == combatants.end() || combatants.isEmpty()){
        extractCombatants();
        return;
    }

    if(!selectedButtons.isEmpty()){
        for(CardButton* button : selectedButtons){
            Card* card = button->cardPtr;
            blockers.append(card);
        }

        selectedButtons.clear();
    }

    if (targetIt != combatants.end()) {
        combatants[targetIt.key()] = blockers;
        targetIt++;
    }

    if (targetIt == combatants.end()){
        extractCombatants();
        return;
    }
}

void MainWindow::botDeclareCombatants(QMap<Card*, QList<Card*>> botCombatants){

    emit sendCombatCards(botCombatants);
    combatants.clear();
    updateUI();
}

void MainWindow::updateUI(){

    activeCards.clear();

    if (!hasWelcomed){
        emit displayGameTip();
        hasWelcomed = true;
    }

    QVector<Zone*> zones;
    ZoneLayout layout;
    Player* currPlayer;
    bool player;

    if(!userPlayer){
        return;
    }

    for (int i = 0; i < 2; i++){

        // Set the zones, layout, and player
        if (i == 0){
            zones = userPlayer->getZones();
            layout = playerLayout;
            currPlayer = userPlayer;
            player = true;
        }
        else{
            zones = enemyPlayer->getZones();
            layout = enemyLayout;
            currPlayer = enemyPlayer;
            player = false;
        }
        if(!currPlayer){
            break;
        }

        layout.landGroups->clear();

        // Go through all zones and update containers
        for (Zone* zone : zones){
            if (zone->type == ZoneType::HAND){
                if(!player){
                }
                updateZone(layout.hand, zone, layout.landGroups, player);
            }
            else if (zone->type == ZoneType::BATTLEFIELD){
                updateZone(layout.battlefield, zone, layout.landGroups, player);
            }
            else if (zone->type == ZoneType::GRAVEYARD){
                updateDeck(zone, layout.graveName, layout.graveyard);
            }
            else if (zone->type == ZoneType::EXILE){
                updateDeck(zone, layout.exileName, layout.exile);
            }
        }

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

        // Set the Health
        layout.health->setText(QString::number(currPlayer->health));

        // Set Phase Label
        layout = playerLayout;
        layout.phaseLabel->setText(QString("Phase: ") + phaseTypeToString(statePointer->currentPhase));

        // Set Active Player Label
        layout.activePlayerLabel->setText(QString(statePointer->player1->isActivePlayer ? "You are " : "The enemy is\n") + "the active player");
        layout.activePlayerLabel->setStyleSheet(statePointer->player1->isActivePlayer
                                                    ? "QLabel { color : green; }"
                                                    : "QLabel { color : red; }");

        handlePhase();
        update();
    }

    //update Stack
    QVBoxLayout* container = ui->stack;

    // Clear current stack view
    QLayoutItem* item;
    while ((item = container->takeAt(0)) != nullptr) {
        if (item->widget()){
            delete item->widget();
        }
        delete item;
    }

    // Re-add current stack contents
    for (const StackObject &object : statePointer->theStack) {
        CardButton* cardButton = createCardButton(object.card, true);
        container->addWidget(cardButton, 0, Qt::AlignHCenter);
    }

    // Set Phase Label
    layout = playerLayout;
    layout.phaseLabel->setText(QString("Phase: ") + phaseTypeToString(statePointer->currentPhase));

    // Set Active Player Label
    layout.activePlayerLabel->setText(QString(statePointer->player1->isActivePlayer ? "You are " : "The enemy is\n") + "the active player");

    handlePhase();
    clearSelection();
    update();
}

void MainWindow::updateZone(QGridLayout* container, Zone* zone, QMap<ManaType, QList<CardButton *>>* landGroups, bool player){
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
        CardButton* cardButton = createCardButton(card, player);
        if(!player && zone->type == ZoneType::HAND){
            cardButton->backCard();
            cardButton->setDisabled(true);
            cardButton->allowHover = false;
        }

        if(card->type == CardType::LAND && zone->type == ZoneType::BATTLEFIELD){
            (*landGroups)[card->color].append(cardButton);
            currentSelectedCard = nullptr;
            continue;
        }

        container->addWidget(cardButton, 0, container->count(), Qt::AlignCenter);
    }
}

void MainWindow::updateDeck(Zone* zone, QString title, QPushButton *deckButton){
    if(!zone){
        return;
    }

    containerCards[title].clear();

    for(Card* card : *zone){
        CardButton* button = createCardButton(card, true);
        containerCards[title].prepend(button);
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

    for (CardButton* card : containerCards[title]) {
        layout->addWidget(card);
        card->enableCard(false);
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
    selectedButtons.clear();
    currentSelectedCard = nullptr;

    for(CardButton* card : activeCards){
        card->setChecked(false);
    }
    update();

}

void MainWindow::extractCombatants(){

    if(combatants.isEmpty() || statePointer->attackers.isEmpty()){
        emit sendCombatCards(combatants);
        return;
    }

    emit sendCombatCards(combatants);

    combatants.clear();
    clearSelection();
}

void MainWindow::handlePhase(){
    if(isTargeting){
        return;
    }

    statePointer->getValidActions();


    for (CardButton* button : activeCards){
        if (!button) {
            continue;
        }
        Card* card = button->cardPtr;

        button->enableCard(card->shouldEnable);
        ui->priorityButton->setEnabled(statePointer->player1->canPassPriority);
        ui->phaseButton->setEnabled(statePointer->player1->canChangePhase);
    }

    // Disable Enemy Hand
    for (int i = 0; i < ui->enemyBattlefield->count(); i ++){
        QLayoutItem* item = ui->enemyBattlefield->itemAt(i);
        QWidget* widget = item->widget();
        CardButton* button = qobject_cast<CardButton*>(widget);
        button->enableCard(false);
    }

    if(statePointer->currentPhase == Phase::DeclareAttackers){
        if(userPlayer->isActivePlayer){
            ui->playCardButton->setText("Declare Attackers");
            ui->playCardButton->setEnabled(true);
            ui->priorityButton->setDisabled(true);
        }
        else {
            ui->playCardButton->setText("Enemy Declaring...");
            ui->playCardButton->setDisabled(true);
            if(userPlayer->holdingPriority){
                statePointer->changePhase();
                updateUI();
            }
        }
    }
    else if(statePointer->currentPhase == Phase::DeclareBlockers){

        if(!userPlayer->isActivePlayer){
            ui->playCardButton->setText("Declare Blockers");
            ui->playCardButton->setEnabled(true);
            ui->priorityButton->setDisabled(true);
            // ui->priorityButton->hide();
        }

        else {
            ui->playCardButton->setText("Enemy Declaring...");
            ui->playCardButton->setDisabled(true);
            // ui->priorityButton->hide();
            if(userPlayer->holdingPriority){
                statePointer->changePhase();
                updateUI();
            }
        }
    }
    else {
        ui->playCardButton->setText("Play");
        ui->playCardButton->setEnabled(true);
        ui->priorityButton->show();
    }
}

void MainWindow::startTargeting(Card *sourceCard){
    isTargeting = true;
    targetSource = sourceCard;
    selectedCards.clear();

    for (CardButton *button : activeCards) {
        button->enableCard(false);
    }

    QGridLayout* targetZone = ui->enemyBattlefield;

    for(int i = 0; i < targetZone->count(); i++){
        QLayoutItem* item = targetZone->itemAt(i);
        QWidget* widget = item->widget();
        CardButton* button = qobject_cast<CardButton*>(widget);
        button->enableCard(true);
    }

    if (targetSource->name.toLower() == "counterspell"){
        for (int i = 0; i < ui->stack->count(); i++){
            QLayoutItem* item = ui->stack->itemAt(i);
            QWidget* widget = item->widget();
            CardButton* button = qobject_cast<CardButton*>(widget);
            button->enableCard(true);
        }
    }

    for (int i = 0; i < ui->enemyHand->count(); ++i) {
        QLayoutItem* item = ui->enemyHand->itemAt(i);
        QWidget* widget = item->widget();

        if (widget && qobject_cast<CardButton*>(widget)) {
            widget->setVisible(false);
        }
    }
    if (targetSource->name.toLower() != "counterspell"){
        ui->enemyTargetButton->show();
    }
}

void MainWindow::stopTargeting(){
    for (int i = 0; i < ui->enemyHand->count(); ++i) {
        QLayoutItem* item = ui->enemyHand->itemAt(i);
        QWidget* widget = item->widget();

        if (widget && qobject_cast<CardButton*>(widget)) {
            widget->setVisible(true);
        }
    }

    isTargeting = false;

    ui->enemyTargetButton->hide();
    targetSource = nullptr;
    clearSelection();
}

void MainWindow::overlayCards(){
    if(!activeCards.isEmpty()){
        for(CardButton* button : activeCards){
            button->resetCard();
        }
    }

    for(int i = 0; i < selectedButtons.count(); i++){
        CardButton* currButton = selectedButtons.at(i);
        QPixmap newIcon = currButton->getOverlayedPixmap(i, Qt::blue);
        currButton->setIcon(QIcon(newIcon));
    }
}

void MainWindow::onGameEnded(bool playerWon) {
    endWorld = new b2World(b2Vec2(0, 0));  // no gravity

    endScene = new QGraphicsScene(this);
    endView = new QGraphicsView(endScene, this);
    endView->setGeometry(this->rect());
    endView->setStyleSheet("background: transparent;");
    endView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    endView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    endView->show();

    for (int i = 0; i < 30; ++i) {
        QPixmap pix(":/Icons/Icons/BackCard.png");
        QGraphicsPixmapItem* card = endScene->addPixmap(pix.scaled(60, 90));
        card->setOffset(-30, -45);
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

CardButton* MainWindow::createCardButton(Card* card, bool player){
    CardButton* cardButton = new CardButton(card);
    activeCards.append(cardButton);

    if(player){
        connect(cardButton, &CardButton::cardSelected, this, &MainWindow::handleCardSelected);
        connect(cardButton, &CardButton::cardTapped, this, &MainWindow::cardTapped);
    }
    connect(cardButton, &CardButton::hovered, this, &MainWindow::updateMagnifier);
    cardButton->setFixedSize(100, 140);

    return cardButton;
}

void MainWindow::displayBlockers(QList<Card*> blockers){
    int i = 1;
    for(Card* card : blockers){
        for(CardButton* button : activeCards){
            if(card == button->cardPtr){
                QPixmap newIcon = button->getOverlayedPixmap(i, Qt::red);
                button->setIcon(QIcon(newIcon));
            }
        }
        i++;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        Settings* settings = new Settings(this);
        connect(settings, &Settings::returnToMainMenu, this, &MainWindow::goToMainMenu);
        settings->exec();
    }
}


void MainWindow::goToMainMenu() {
    emit returnToMainMenu();
    this->close();
    this->deleteLater();
}

void MainWindow::lotusHandling(){
    LotusDialog* lotus = new LotusDialog();
    int result = lotus->exec();  // Show the dialog

    if (result == QDialog::Accepted) {
        ManaType selectedMana = lotus->getData();
        QMap<ManaType, int> mana;
        mana[selectedMana] = 3;
        statePointer->player1->addMana(&mana);
    }

    delete lotus;
}
