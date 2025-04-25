#include "gamestate.h"
#include "bot.h"

GameState::GameState(){
    currentPhase = Phase::Untap;
}

void GameState::changePhase(){
    bool hasSeenPhase = false;
    if (currentPhaseIndex >= phases.size() - 1){
        currentPhaseIndex = 0;
    }
    else{
        currentPhaseIndex ++;
    }
    currentPhase = phases[currentPhaseIndex];
    if (currentPhase == Phase::Untap){
        player1->emptyManaPool();
        player2->emptyManaPool();
        player1->untap();
        player2->untap();
    }
    else if (currentPhase == Phase::Upkeep){
        player1->upkeepPhase();
        player2->upkeepPhase();
    }
    else if (currentPhase == Phase::Draw){
        if (player1->isActivePlayer && turnCount != 0){
            player1->drawCard();
        }
        else if (player2->isActivePlayer && turnCount != 0){
            player2->drawCard();
        }
    }
    else if(currentPhase == Phase::PreCombatMain){
        player1->emptyManaPool();
        player2->emptyManaPool();
        attackers.clear();
    }
    else if(currentPhase == Phase::BeginCombat){
        player1->emptyManaPool();
        player2->emptyManaPool();
    }
    else if(currentPhase == Phase::DeclareAttackers){
        if(player2->isActivePlayer){
            Bot* botPlayer = static_cast<Bot*>(player2);
            botPlayer->takeTurn(this);
        }
    }
    else if(currentPhase == Phase::DeclareBlockers){
        if(!player2->isActivePlayer){
            Bot* botPlayer = static_cast<Bot*>(player2);
            botPlayer->takeTurn(this);
        }
    }
    else if(currentPhase == Phase::PostCombatMain){
        player1->emptyManaPool();
        player2->emptyManaPool();
    }
    else if (currentPhase == Phase::EndStep){
        player1->emptyManaPool();
        player2->emptyManaPool();
        player1->endStepPhase();
        player2->endStepPhase();
    }
    else if (currentPhase == Phase::Cleanup){
        player1->emptyManaPool();
        player2->emptyManaPool();
        player1->cleanupPhase();
        player2->cleanupPhase();

        // Checks if players have more than 7 cards
        player1->endTurn();
        player2->endTurn();

        changeActivePlayer();
        turnCount += 1;
        changePhase();
        if (player2->isActivePlayer) {
            Bot* botPlayer = static_cast<Bot*>(player2);
            botPlayer->takeTurn(this);
        }
    }
}

void GameState::changePriority(){
    if(player1->holdingPriority){
        player1->madeAction = false;
        player2->holdingPriority = true;
        player1->holdingPriority = false;
    }
    else{
        player2->madeAction = false;
        player1->holdingPriority = true;
        player2->holdingPriority = false;
    }
}

void GameState::changeActivePlayer(){
    if (player1->isActivePlayer){
        player2->holdingPriority = true;
        player1->holdingPriority = false;
        player2->isActivePlayer = true;
        player1->isActivePlayer = false;
    }
    else{
        player1->holdingPriority = true;
        player2->holdingPriority = false;
        player1->isActivePlayer = true;
        player2->isActivePlayer = false;
    }
}

void GameState::resolveCombatDamage(QMap<Card*, QList<Card*>> CombatCreatures){
    Player* attackingPlayer;
    Player* defendingPlayer;
    if (player1->isActivePlayer){
        attackingPlayer = player1;
        defendingPlayer = player2;
    }
    else{
        attackingPlayer = player2;
        defendingPlayer = player1;
    }
    for (Card* attacker : CombatCreatures.keys()){
        // Holds the attackers power, whenever damage is delt to a creature the excess is stored here
        // int powerLeftToDeal = attacker->power;

        // If the attacker has no blockers, deal damage to the player equal to its toughness
        if (CombatCreatures[attacker].empty()){
            defendingPlayer->takeDamage(attacker->power);
        }
        // Otherwise deal damage to the blocking creatures with powerLeftToDeal
        else{

            int remainingPower = attacker->power;

            // Step 1: Assign and deal attacker's damage to blockers in order
            for (Card* blocker : CombatCreatures[attacker]) {
                if (remainingPower <= 0)
                    break;

                int damageToDeal = std::min(remainingPower, blocker->toughness);
                blocker->takeDamage(damageToDeal);
                remainingPower -= damageToDeal;
            }

            // Step 2: Each blocker also deals damage to the attacker
            for (Card* blocker : CombatCreatures[attacker]) {
                attacker->takeDamage(blocker->power);
            }

            // Step 3: Check for deaths
            for (Card* blocker : CombatCreatures[attacker]) {
                if (blocker->currHealth <= 0) {
                    defendingPlayer->moveCardString(blocker, "battlefield", "graveyard", true);
                }
            }
            if (attacker->currHealth <= 0) {
                attackingPlayer->moveCardString(attacker, "battlefield", "graveyard", true);
            }
        }
    }
    changePhase();
}

void GameState::addToStack(StackObject stackObject)
{
    theStack.append(stackObject);
}

void GameState::resolveStack(){
    if (!theStack.empty()){
        StackObject stackObject = theStack.takeLast();
        if (!std::holds_alternative<std::nullptr_t>(stackObject.target)){
            if(std::holds_alternative<Player*>(stackObject.target)){
                Player* t = get<Player*>(stackObject.target);
                stackObject.card->ability.use(t);
            }
            else if(std::holds_alternative<Card*>(stackObject.target)){
                Card* c = get<Card*>(stackObject.target);
                if (stackObject.card->isCountered){
                    // Loser got their spell countered
                }
                else{
                    stackObject.card->ability.use(c);
                }
                if(c->currHealth <= 0){
                    if (player1->Battlefield.findCard(c)){
                        player1->moveCardString(c, "battlefield", "graveyard", true);
                    }
                    else{
                        player2->moveCardString(c, "battlefield", "graveyard", true);
                    }
                }
            }
        }
        if (!stackObject.card->isCountered){
            if (stackObject.card->isPermanent){
                stackObject.player->moveCardString(stackObject.card, "hand", "battlefield", false);
            }
            else{
                stackObject.player->moveCardString(stackObject.card, "hand", "graveyard", true);
            }
        }
        else{
            if (player1->Hand.findCard(stackObject.card)){
                player1->moveCardString(stackObject.card, "hand", "graveyard", true);
            }
            else{
                player2->moveCardString(stackObject.card, "hand", "graveyard", true);
            }
        }
        stackObject.card->isOnStack = false;
    }
}

PhaseRules GameState::getPhaseRules(){
    switch(currentPhase){
    case Phase::Untap:
        return PhaseRules{false, false, false, false, false, false, true};
        break;
    case Phase::Upkeep:
        // Player Upkeep trigger
        return PhaseRules{true, false, false, false, false, false, false};
        break;
    case Phase::Draw:
        return PhaseRules{false, false, true, false, false, false, false};
        // Player can draw a card
        break;
    case Phase::PreCombatMain:
        return PhaseRules{true, true, true, false, false, false, false};
        // Player can play a land, cast a card or activate an ability
        break;
    case Phase::BeginCombat:
        return PhaseRules{true, false, false, false, false, false, false};
        // Players may play instant speed cards or activate abilites
        break;
    case Phase::DeclareAttackers:
        return PhaseRules{false, false, false, true, false, false, false};
        // ActivePlayer selects attacking creatures
        break;
    case Phase::DeclareBlockers:
        return PhaseRules{false, false, false, false, true, false, false};
        // PassivePlayer selects defending creatures
        break;
    case Phase::CombatDamage:
        return PhaseRules{false, false, false, false, false, false, false};
        // Based on the delcared attackers and blockers, resolve the damage
        break;
    case Phase::PostCombatMain:
        return PhaseRules{true, true, true, false, false, false, false};
        // Players can play a land, cast a card or activate an ability
        break;
    case Phase::EndStep:
        return PhaseRules{true, false, false, false, false, false, false};
        // Player endstep trigger
        break;
    case Phase::Cleanup:
        return PhaseRules{false, false, false, false, false, true, false};
        // Player discards excess cards
        break;
    default:
        return PhaseRules{}; // return default (all false)
    }
}
bool GameState::stackIsEmpty(){
    return theStack.isEmpty();
}
QString GameState::toString(){
    return "";
}
Player* GameState::getPriorityPlayer(){
    if (player1->holdingPriority){
        return player1;
    }
    else{
        return player2;
    }
}

void GameState::validateHand(Player* player){
    PhaseRules rules = getPhaseRules();
    for (Card* card : player->Hand){
        if (rules.canPlayInstant && card->type == CardType::INSTANT && player->canPayMana(card) && !card->isOnStack){
            card->shouldEnable = true;
        }
        else if (player->isActivePlayer && rules.canPlaySorcery && card->type == CardType::SORCERY && player->canPayMana(card) && stackIsEmpty() && !card->isOnStack){
            card->shouldEnable = true;
        }
        else if (player->isActivePlayer && rules.canPlaySorcery && card->type == CardType::LAND && stackIsEmpty() && !player->hasPlayedLand){
            card->shouldEnable = true;
        }
        else if (player->isActivePlayer && rules.canPlaySorcery && stackIsEmpty() && player->canPayMana(card) && !card->isOnStack &&
                 (card->type == CardType::CREATURE ||
                  card->type == CardType::ARTIFACT ||
                  card->type == CardType::ENCHANTMENT ||
                  card->type == CardType::PLANESWALKER ||
                  card->type == CardType::BATTLE)) {
            card->shouldEnable = true;
        }
        else{
            card->shouldEnable = false;
        }
    }
}

void GameState::validateBattlefield(Player* player){
    PhaseRules rules = getPhaseRules();
    for (Card* card : player->Battlefield){
        card->shouldEnable = false;

        if (player->isActivePlayer && rules.canDeclareAttack && card->type == CardType::CREATURE && !card->isTapped && (!card->hasSummoningSickness or !player->hasSummoningSickness)){
            card->shouldEnable = true;
        }
        if (!player->isActivePlayer && rules.canDeclareDefense && card->type == CardType::CREATURE){
            card->shouldEnable = true;
        }
        if (card->type == CardType::LAND){
            card->shouldEnable = true;
        }
        if (card->type == CardType::ARTIFACT){
            card->shouldEnable = true;
        }
    }
}

void GameState::validatePlayerActions(Player* player){
    player->canDrawCard = false;
    player->canPassPriority = false;
    player->canChangePhase = false;

    if (player->holdingPriority){
        player->canPassPriority = true;
    }
    if (player->isActivePlayer && theStack.empty() && player->holdingPriority){
        player->canChangePhase = true;
    }
}

void GameState::getValidActions(){
    validateHand(player1);
    validateHand(player2);
    validateBattlefield(player1);
    validateBattlefield(player2);
    validatePlayerActions(player1);
    validatePlayerActions(player2);
}
