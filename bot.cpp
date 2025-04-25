#include "bot.h"
#include "gamestate.h"
#include "command.h"
#include <QDebug>
#include <QTimer>

Bot::Bot(QStringList deckList) : Player(deckList) {
    hasSummoningSickness = true;
    hasPlayedLand = false;
    hasntDrawnForTurn = false;
    madeAction = false;
}

void Bot::takeTurn(GameState* gameState) {
    //QTimer::singleShot(3000, this, [this, gameState]() {
    switch (gameState->currentPhase) {
    case Phase::PreCombatMain:
        playCard(gameState);
        passPriority(gameState);
        break;
    case Phase::DeclareAttackers:
        declareAttackers(gameState);
        changePhase(gameState);
        break;
    case Phase::DeclareBlockers:
        declareBlockers(gameState);
        changePhase(gameState);
        break;
    case Phase::PostCombatMain:
        playCard(gameState);
        passPriority(gameState);
        break;
    case Phase::EndStep:
        endTurn(gameState);
        passPriority(gameState);
        break;
    default:
        // Any other phase has no special actions to auto take
        if (this->isActivePlayer) {
            changePhase(gameState);
        }
        passPriority(gameState);
        break;
    }
    //});
}

void Bot::playCard(GameState* gameState) {
    if (!hasPlayedLand && isActivePlayer) {
        for (Card* card : Hand) {
            if (card->isLand) {
                qDebug() << "Chose to play " << card->name << " for the land part";
                playCardCommand cmd(gameState, card, nullptr);
                cmd.execute();
                hasPlayedLand = true;
                break;
            }
        }
    }

    QVector<Card*> playableCards;
    for (Card* card : Hand) {
        if (card->type == CardType::LAND && hasPlayedLand){
            continue;
        }
        if (card->shouldEnable) {
            playableCards.append(card);
        }
    }

    std::sort(playableCards.begin(), playableCards.end(), [this](Card* a, Card* b) {
        return calculateManaValue(a) > calculateManaValue(b);
    });

    if (!playableCards.isEmpty()) {
        for (Card* chosen : playableCards){
            int manaNeeded = calculateManaValue(chosen);

            QVector<Card*> untappedLands;
            for (Card* land : Battlefield) {
                if (land->type == CardType::LAND && !land->isTapped) {
                    untappedLands.append(land);
                }
            }

            if (untappedLands.size() < manaNeeded) {
                qDebug() << "Not enough untapped lands to play " << chosen->name;
                continue;
            }

            if (untappedLands.size() < manaNeeded && chosen == playableCards.last()) {
                qDebug() << "Not enough untapped lands to play anything" << chosen->name;
                return;
            }

            for (int i = 0; i < manaNeeded; ++i) {
                Card* land = untappedLands[i];
                land->isTapped = true;
                qDebug() << "Tapped land " << land->name << " to pay for " << chosen->name;
            }

            qDebug() << "Chose to play " << chosen->name << " (paid " << manaNeeded << " mana)";
            if (chosen->needsTarget){
                if (chosen->name.toLower() == "counterspell"){
                    if (gameState->theStack.empty()){
                        continue;
                    }
                    else{
                        for (StackObject stackItem : gameState->theStack){
                            if (stackItem.player == gameState->player2){
                                continue;
                            }
                            else{
                                playCardCommand* cmd = new playCardCommand(gameState, chosen, stackItem.card);
                                cmd->execute();
                            }
                        }
                    }
                }
                else if(chosen->type == CardType::INSTANT){
                    if (gameState->player1->Battlefield.getCount() == 0){
                        playCardCommand* cmd = new playCardCommand(gameState, chosen, gameState->player1);
                        cmd->execute();
                    }
                    else{
                        for (Card* card : gameState->player1->Battlefield){
                            qDebug() << card->name;
                            if (chosen->name.toLower() == "shock"){
                                qDebug() << "Cast shock";
                                if (card->toughness <= 2 && card->type == CardType::CREATURE){
                                    playCardCommand* cmd = new playCardCommand(gameState, chosen, card);
                                    cmd->execute();
                                    qDebug() << "Cast on creature";
                                }
                                else{
                                    continue;
                                }
                            }
                            else{
                                if (card->toughness <= 3 && card->type == CardType::CREATURE){
                                    playCardCommand* cmd = new playCardCommand(gameState, chosen, card);
                                    cmd->execute();
                                    qDebug() << "Cast on creature";
                                }
                                else{
                                    continue;
                                }
                            }
                        }
                    }
                }
                else{
                    playCardCommand* cmd = new playCardCommand(gameState, chosen, gameState->player2);
                    cmd->execute();
                }
            }
            else {
                playCardCommand cmd(gameState, chosen, nullptr);
                cmd.execute();
            }
            break;
        }
    }

    if (this->isActivePlayer && !gameState->player1->madeAction && gameState->theStack.empty()) {
        changePhase(gameState);
    }
}


int Bot::calculateManaValue(Card* card) {
    int total = 0;
    for (auto [color, amount] : card->cost.toStdMap()) {
        total += amount;
    }
    return total;
}

void Bot::declareAttackers(GameState* gameState) {
    // if(!this->isActivePlayer){
    //     return;
    // }

    QMap<Card*, QVector<Card*>> combatCreatures;

    for (Card* creature : Battlefield) {
        if (creature->type == CardType::CREATURE && !creature->isTapped) {
            combatCreatures[creature] = QVector<Card*>();
            creature->isTapped = true;
        }
    }

    QList<Card*> attackers;
    for (Card* card : combatCreatures.keys()){
        attackers.append(card);
    }
    gameState->attackers = combatCreatures.keys();
    emit UiDeclareAttackers(combatCreatures.keys());
}

void Bot::declareBlockers(GameState* gameState) {

    Player* attacker = gameState->player1;
    QMap<Card*, QVector<Card*>> combatCreatures;

    QList<Card*> attackingCreatures = gameState->attackers;

    if (attackingCreatures.isEmpty()) {
        emit UiDeclareCombatants(combatCreatures);
        return;
    }

    QVector<Card*> availableBlockers;
    for (Card* card : Battlefield) {
        if (card->type == CardType::CREATURE && !card->isTapped && !card->hasSummoningSickness) {
            availableBlockers.append(card);
            card->isTapped = true;
        }
    }

    std::sort(attackingCreatures.begin(), attackingCreatures.end(), [](Card* a, Card* b) {
        return a->power > b->power;
    });

    std::sort(availableBlockers.begin(), availableBlockers.end(), [](Card* a, Card* b) {
        return a->power < b->power;
    });

    for (Card* attacker : attackingCreatures) {
        combatCreatures[attacker];
        for (int i = 0; i < availableBlockers.size(); i++) {
            Card* blocker = availableBlockers[i];
            if (blocker->toughness > attacker->power ||
                (blocker->power >= attacker->toughness && blocker->toughness >= attacker->power)) {

                combatCreatures[attacker].append(blocker);
                availableBlockers.removeAt(i);
                emit showBlockers(combatCreatures[attacker]);
                break;
            }
        }
    }

    emit UiDeclareCombatants(combatCreatures);
}

void Bot::endTurn(GameState* gameState) {
    changePhaseCommand cmd(gameState);
    cmd.execute();
}

void Bot::passPriority(GameState* gameState) {
    passPriorityCommand cmd(gameState);
    cmd.execute();
}

void Bot::changePhase(GameState* gameState){
    changePhaseCommand cmd(gameState);
    cmd.execute();
}
