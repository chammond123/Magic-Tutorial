// #include "gamemanager.h"

#include "gamemanager.h"
#include "qobject.h"
gamemanager::gamemanager(QObject *parent)
    : QObject{parent}
{
    state = new GameState();
}

gamemanager::~gamemanager(){
    delete state;
}

// UI Player Commands
void gamemanager::onPlayCard(Card *card, Card* target){
    if(!card){
        qDebug() << "NO CARD";
        return;
    }
    playCardCommand cmd = playCardCommand(state, card, target);
    cmd.execute();
    emit updateUI();
}

void gamemanager::onPassPriority(){
    qDebug() << "Received priority command";
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
