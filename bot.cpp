#include "bot.h"
#include "gamestate.h"
#include "command.h"
#include "carddictionary.h"

Bot::Bot(QStringList deckList) : Player(deckList) {}

void Bot::takeTurn(GameState* gameState) {
    switch (gameState->currentPhase) {
    case Phase::PreCombatMain:
        playCard(gameState);
        break;
    case Phase::DeclareAttackers:
        declareAttackers(gameState);
        break;
    case Phase::DeclareBlockers:
        declareBlockers(gameState);
        break;
    case Phase::PostCombatMain:
        playCard(gameState);
        break;
    case Phase::EndStep:
        endTurn(gameState);
        break;
    default:
        passTurn(gameState);
        break;
    }
}

void Bot::playCard(GameState* gameState) {
    if (!hasPlayedLand) {
        for (Card* card : Hand) {
            if (card->type == CardType::LAND) {
                playCardCommand cmd(gameState, card, nullptr);
                cmd.execute();
                hasPlayedLand = true;
                return;
            }
        }
    }

    QVector<Card*> playableCards;
    for (Card* card : Hand) {
        if (canPayMana(card)) {
            playableCards.append(card);
        }
    }

    std::sort(playableCards.begin(), playableCards.end(), [this](Card* a, Card* b) {
        return calculateManaValue(a) > calculateManaValue(b);
    });

    if (!playableCards.isEmpty()) {
        playCardCommand cmd(gameState, playableCards.first(), nullptr);
        cmd.execute();
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
    QMap<Card*, QVector<Card*>> combatCreatures;

    for (Card* creature : Battlefield) {
        if (creature->type == CardType::CREATURE && !creature->isTapped && !creature->hasSummoningSickness) {
            combatCreatures[creature] = QVector<Card*>();
        }
    }

    if (!combatCreatures.isEmpty()) {
        declareCombatCommand cmd(gameState, combatCreatures);
        cmd.execute();
    }
}

void Bot::declareBlockers(GameState* gameState) {
    Player* attacker = gameState->player1;
    QMap<Card*, QVector<Card*>> combatCreatures;

    QVector<Card*> attackingCreatures;
    for (Card* card : attacker->Battlefield) {
        if (card->type == CardType::CREATURE) {
            attackingCreatures.append(card);
        }
    }

    if (attackingCreatures.isEmpty()) return;

    QVector<Card*> availableBlockers;
    for (Card* card : Battlefield) {
        if (card->type == CardType::CREATURE && !card->isTapped) {
            availableBlockers.append(card);
        }
    }

    std::sort(attackingCreatures.begin(), attackingCreatures.end(), [](Card* a, Card* b) {
        return a->power > b->power;
    });

    std::sort(availableBlockers.begin(), availableBlockers.end(), [](Card* a, Card* b) {
        return a->power < b->power;
    });

    for (Card* attacker : attackingCreatures) {
        for (int i = 0; i < availableBlockers.size(); i++) {
            Card* blocker = availableBlockers[i];
            if (blocker->toughness > attacker->power ||
                (blocker->power >= attacker->toughness && blocker->toughness >= attacker->power)) {

                combatCreatures[attacker].append(blocker);
                availableBlockers.removeAt(i);
                break;
            }
        }
    }

    if (!combatCreatures.isEmpty()) {
        declareCombatCommand cmd(gameState, combatCreatures);
        cmd.execute();
    }
}

void Bot::endTurn(GameState* gameState) {
    changePhaseCommand cmd(gameState);
    cmd.execute();
}

void Bot::passTurn(GameState* gameState) {
    passPriorityCommand cmd(gameState);
    cmd.execute();
}
