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

void gamemanager::displayTip(QString tip){
    GameTipsDialog* tipDialog = new GameTipsDialog(tip);
    tipDialog->exec();
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
}

void gamemanager::onPassPriority(){
    qDebug() << "Received priority command";
    // displayTip("This is a test");
    passPriorityCommand cmd = passPriorityCommand(state);
    cmd.execute();
    emit updateUI();
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
}

void gamemanager::onCombatCardsReceived(QMap<Card*, QVector<Card*>> CombatCreatures){
    declareCombatCommand cmd = declareCombatCommand(state, CombatCreatures);
    cmd.execute();
    emit updateUI();
}

void gamemanager::onTapCard(Card* card){
    tapCardCommand cmd = tapCardCommand(state, card);
    cmd.execute();
    emit updateUI();
}
// // Player object communication
