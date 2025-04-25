
#include "gamemanager.h"
#include "qobject.h"
#include "gametipsdialog.h"
gamemanager::gamemanager(QObject *parent)
    : QObject{parent}
{
    state = new GameState();
}

gamemanager::~gamemanager(){
    delete state;
}

void gamemanager::displayTip(QString tip, int xCoord, int yCoord){    
    GameTipsDialog* tipDialog = new GameTipsDialog(tip);
    tipDialog->move(xCoord, yCoord);
    tipDialog->exec();
}

void gamemanager::onReceiveMainWindowPos(int left, int right, int top, int bottom){
    mainWindowLeft = left;
    mainWindowTop = top;
    mainWindowRight = right;
    mainWindowBottom = bottom;
}

void gamemanager::displayPhaseTip() {
    emit requestMainWindowPos();
    QString landName = "";
    QString creatureName = "any cards with any numbered mana requirement";
    bool hasInstant = false;
    QString phaseMessage = ""; // Initialize the message for each phase
    if (state->player1->isActivePlayer){
        if (displayGameTips) {
            int midX = (mainWindowLeft + mainWindowRight) / 2;
            int midY = (mainWindowTop + mainWindowBottom) / 2;

            // Check if the player has instants in hand
            for (Card* c : state->player1->Hand) {
                if (c->type == CardType::INSTANT) {
                    hasInstant = true;
                    break;
                }
            }

            switch(state->currentPhase) {
            case Phase::Untap:
                if(!hasDisplayedWelcome){
                    phaseMessage = "Welcome to Magic The Gathering!\n"
                                   "If you have not yet read the tutorial pages, please do so.\n"
                                   "The goal of the game is to get your opponent's life total to 0. \n"
                                   "You can do that by playing land cards, casting creatures and dealing damage with spells.\n"
                                   "There's a lot to learn, and we will go over every phase when we get to it.\n"
                                   "For now, close me and hit the 'Next Phase' button. \n"
                                   "It is what you will use to switch phases in this tutorial program from here on out!";
                    hasDisplayedWelcome = true;
                }
                else{
                    phaseMessage = "UNTAP PHASE\n"
                                   "All your tapped cards (turned sideways) now untap (turn straight).\n"
                                   "This includes lands, creatures, and other permanents.\n"
                                   "Players can't cast spells or activate abilities during this phase\n"
                                   "except those that specifically trigger during untap.";
                }
                break;

            case Phase::Upkeep:
                phaseMessage = "UPKEEP PHASE\n"
                               "This is when many triggered abilities happen that say\n"
                               "'at the beginning of your upkeep'. Pay attention to\n"
                               "cards that require upkeep costs or have effects that\n"
                               "trigger now. This is your first chance to play instants\n"
                               "and activate abilities each turn.";
                break;

            case Phase::Draw:
                phaseMessage = "DRAW PHASE\n"
                               "Here, you would draw one card from the top of your library.\n"
                               "For this tutorial, cards will be drawn automatically.\n"
                               "The player who goes first skips their first draw phase.\n"
                               "If you can't draw because your library is empty, you lose the game!\n"
                               "Some effects may allow you to draw additional cards.";
                break;

            case Phase::PreCombatMain:
                for(Card* c : state->player1->Hand){
                    if(c->type == CardType::LAND){
                        landName = c->name;
                    }
                }
                for(Card* c : state->player1->Hand){
                    if(c->type != CardType::LAND && ((c->color == ManaType::RED && landName == "Mountain") || (c->color == ManaType::BLUE && landName == "Island"))){
                        creatureName = "a " + c->name;
                    }
                }
                phaseMessage = "PRE-COMBAT MAIN PHASE\n"
                               "This is when you can play most of your cards:\n"
                               "• You may play ONE land card (if you haven't already this turn)\n"
                               "• You may cast any sorceries, creatures, enchantments,\n"
                               "  artifacts, or planeswalkers if you have enough mana\n"
                               "• Creatures cast now can attack this turn if they don't have\n"
                               "  summoning sickness (meaning you can not attack with cards that have just been played)\n"
                               "Maybe try playing a " + landName + " land card!\n"
                                            "It will help you play " + creatureName + "!";
                break;

            case Phase::BeginCombat:
                phaseMessage = "BEGIN COMBAT PHASE\n"
                               "This short phase marks the transition into combat.\n"
                               "Last chance for players to cast spells or activate abilities\n"
                               "before attackers are declared. Some cards have abilities that\n"
                               "trigger specifically at this time, like 'at the beginning of combat'.\n"
                               "This is your opponent's last chance to tap your creatures to\n"
                               "prevent them from attacking.";
                break;

            case Phase::DeclareAttackers:
                phaseMessage = "DECLARE ATTACKERS PHASE\n"
                               "Choose which of YOUR untapped creatures will attack and who/what\n"
                               "they're attacking (opponent or their planeswalkers).\n"
                               "• Tap all attacking creatures (unless they have Vigilance)\n"
                               "• You can't attack with creatures that have 'summoning sickness'\n"
                               "  (those played this turn), unless they have Haste\n"
                               "• You don't have to attack with any creatures if you don't want to\n"
                               "• Once attackers are declared, players can cast instants or activate abilities";
                break;

            case Phase::DeclareBlockers:
                phaseMessage = "DECLARE BLOCKERS PHASE\n"
                               "Your opponent decides if they want to block your attacks:\n"
                               "• They can assign their untapped creatures as blockers\n"
                               "• Multiple creatures can block a single attacker\n"
                               "• Each creature can only block one attacker (unless it has special abilities)\n"
                               "• Creatures with Flying can only be blocked by creatures with Flying or Reach\n"
                               "• Attackers with Trample deal excess damage to the player\n"
                               "• After blockers are declared, players can cast instants or activate abilities";

                break;

            case Phase::CombatDamage:
                phaseMessage = "COMBAT DAMAGE PHASE\n"
                               "All combat damage is assigned and dealt simultaneously:\n"
                               "• Unblocked creatures deal damage to player/planeswalker\n"
                               "• Blocked creatures deal damage to blocking creatures\n"
                               "• Blocking creatures deal damage to attacking creatures\n";
                break;

            case Phase::PostCombatMain:
                phaseMessage = "POST-COMBAT MAIN PHASE\n"
                               "Your second main phase works exactly like the first main phase:\n"
                               "• You may play your ONE land for the turn (if you haven't already)\n"
                               "• You may cast any sorceries, creatures, enchantments, etc.\n"
                               "• Creatures cast now will have summoning sickness until your next turn\n"
                               "• This is often a good time to play creatures you don't want to\n"
                               "  risk exposing to combat tricks";

                break;

            case Phase::EndStep:
                phaseMessage = "END STEP\n"
                               "This marks the formal end of your turn:\n"
                               "• 'At the end of turn' or 'until end of turn' effects happen or expire now\n"
                               "• Both players can still cast instants and activate abilities\n"
                               "• If you have more than seven cards in hand, prepare to discard down\n"
                               "  to seven in the next phase\n"
                               "• Some effects might trigger here";

                break;

            case Phase::Cleanup:
                phaseMessage = "CLEANUP PHASE\n"
                               "The final phase of your turn:\n"
                               "• Discard down to your maximum hand size (usually seven cards)\n"
                               "• All damage on creatures is removed\n"
                               "• 'Until end of turn' and 'this turn' effects end now\n"
                               "• Players typically can't cast spells during this phase\n"
                               "• The turn will pass to your opponent after this phase";
                break;

            default:
                qDebug() << "";
            }

            if (hasInstant) {
                phaseMessage += "\nYou also have instants in your hand, which you can cast during this phase if you can pay the mana";
            }
            displayTip(phaseMessage, midX - 30, midY + 30);
        }
    }

}

// UI Player Commands
void gamemanager::onPlayCard(Card *card, std::variant<Player*, Card*, std::nullptr_t> target){
    if(!card){
        return;
    }

    if(card->needsTarget && std::holds_alternative<std::nullptr_t>(target)){
        emit promptTargeting(card);
        return;
    }

    playCardCommand cmd = playCardCommand(state, card, target);
    cmd.execute();
    emit updateUI();
}

void gamemanager::onPassPriority(){
    passPriorityCommand cmd = passPriorityCommand(state);
    cmd.execute();
    emit updateUI();
    if (state->player1->health <= 0){
        emit gameOver(false);
    }
    else if (state->player2->health <= 0){
        emit gameOver(true);
    }
}

void gamemanager::onChangePhase(){
    QVector<Phase> playerActionPhases = {Phase::PreCombatMain, Phase::PostCombatMain, Phase::DeclareAttackers, Phase::DeclareBlockers, Phase::EndStep};
    changePhaseCommand cmd = changePhaseCommand(state);
    cmd.execute();
    emit updateUI();
    displayPhaseTip();
    if(!playerActionPhases.contains(state->currentPhase) &&
        !(state->currentPhase == Phase::Upkeep && state->player1->Hand.containsEnabledType(CardType::INSTANT))){
        onChangePhase();
    }
}

void gamemanager::onCombatCardsReceived(QMap<Card*, QVector<Card*>> CombatCreatures){
    declareCombatCommand cmd = declareCombatCommand(state, CombatCreatures);
    cmd.execute();
    if(state->player1->health <= 0){
        emit gameOver(false);
    }
    else if(state->player2->health <= 0){
        emit gameOver(true);
    }
    emit updateUI();
}

void gamemanager::onTapCard(Card* card){
    tapCardCommand cmd = tapCardCommand(state, card);
    cmd.execute();
    emit updateUI();
}

void gamemanager::onToggleGameTips(bool toggled){
    displayGameTips = toggled;
}
// // Player object communication
