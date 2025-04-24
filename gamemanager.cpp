// #include "gamemanager.h"

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

void gamemanager::updateDialog(){
    if (displayGameTips){
        switch(state->currentPhase){
        case Phase::Untap:
            break;
        case Phase::Upkeep:
            displayTip("Welcom to Magic The Gathering!\n"
                       "The goal of the game is to get your opponants\n"
                       "life total below 20. You can do that by playing land\n"
                       "cards, casting creatures and slaying with spells.\n"
                       "Close me to begin an introduction to the phases of the game.", 1200, 500);
            displayTip("Phases are point during the game where\n"
                       "certain actions by players are allowed or disalowed.\n"
                       "The game will progress through these phases, allowing\n"
                       "you and your opponent to interact in different ways.\n"
                       "If that sounds counfusing thats ok, this toutorial will\n"
                       "wallk you through the phases in a game of Magic The Gathering.", 1200, 550);
            break;
        case Phase::Draw:
            displayTip("Every turn you will draw a card\n"
                       "from the top of your library to \n"
                       "your hand during the draw phase.", 1200, 950);
            break;
        case Phase::PreCombatMain:
            displayTip("During your pre-combat main phase you are allowed\n"
                       "to play one land per turn as well as any other card\n"
                       "you can afford to pay for with your mana", 700, 950);
            break;
        case Phase::BeginCombat:
            displayTip("This phase marks the beginning of whats called comabat.\n"
                       "This is where your creatures have a chance to damage your\n"
                       "opponent.", 1400, 600);
            break;
        case Phase::DeclareAttackers:
            displayTip("During this phase you are allowed to select creatures\n"
                       "that you want to attack your opponent. Be careful if\n"
                       "they control any creatures of their own they can block\n"
                       "your attack!", 1200, 1200);
            break;
        case Phase::DeclareBlockers:
            displayTip("This phase is when your opponent will decidec if \n"
                       "they want to block an attack from your creatures.\n"
                       "Any unblocked creatures will deal damage to your opponent.", 1400, 700);
            break;
        case Phase::CombatDamage:
            displayTip("The chosen attacking and blocking creatures will \n"
                       "now deal damage to their repsective targets.", 1400, 700);
            break;
        case Phase::PostCombatMain:
            displayTip("This phase may look familiar. It is your second main phase\n"
                       "You may play a land if you haven't done so for turn yet and\n"
                       "as well as any spells that you can afford.", 700, 950);
            break;
        case Phase::EndStep:
            displayTip("This marks the end of your turn, the next phase will reset\n"
                       "the health of your creature cards.", 1500, 700);
            break;
        case Phase::Cleanup:
            displayTip("Changing phase one more time will pass the turn to your opponent.", 1500, 950);
            break;
        default:
            qDebug() << "Warning: Unhandled phase in getPhaseRules!";
        }
    }
}

// UI Player Commands
void gamemanager::onPlayCard(Card *card, std::variant<Player*, Card*, std::nullptr_t> target){
    if(!card){
        qDebug() << "NO CARD";
        return;
    }

    if(card->needsTarget && std::holds_alternative<std::nullptr_t>(target)){
        emit promptTargeting(card);
        return;
    }

    playCardCommand cmd = playCardCommand(state, card, target);
    cmd.execute();
    emit updateUI();
    updateDialog();
}

void gamemanager::onPassPriority(){
    qDebug() << "Received priority command";
    // displayTip("This is a test");
    passPriorityCommand cmd = passPriorityCommand(state);
    cmd.execute();
    emit updateUI();
    updateDialog();
    if (state->player1->health <= 0){
        emit gameOver(false);
    }
    else if (state->player2->health <= 0){
        emit gameOver(false);
    }
}

void gamemanager::onChangePhase(){
    changePhaseCommand cmd = changePhaseCommand(state);
    cmd.execute();
    emit updateUI();
    updateDialog();
}

void gamemanager::onCombatCardsReceived(QMap<Card*, QVector<Card*>> CombatCreatures){
    declareCombatCommand cmd = declareCombatCommand(state, CombatCreatures);
    cmd.execute();
    emit updateUI();
    updateDialog();
}

void gamemanager::onTapCard(Card* card){
    tapCardCommand cmd = tapCardCommand(state, card);
    cmd.execute();
    emit updateUI();
    updateDialog();
}

void gamemanager::onToggleGameTips(bool toggled){
    displayGameTips = toggled;
}
// // Player object communication
