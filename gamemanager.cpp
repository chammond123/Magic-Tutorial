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
void gamemanager::onCardDrawn(){
    drawCommand cmd = drawCommand(state);
    if (cmd.isValid()){
        cmd.execute();
        emit updateUI();
    }
}

void gamemanager::onPlayCard(Card *card, Card* target){
    playCardCommand cmd = playCardCommand(state, card, target);
    if (cmd.isValid()){
        cmd.execute();
        emit updateUI();
    }
}

void gamemanager::onPassPriority(){
    qDebug() << "Received priority command";
    passPriorityCommand cmd = passPriorityCommand(state);
    cmd.execute();
    emit updateUI();
}

void gamemanager::onChangePhase(){
    changePhaseCommand cmd = changePhaseCommand(state);
    cmd.execute();
    emit updateUI();
        // emit updateUIPhase();
}

void gamemanager::onCombatCardsReceived(QMap<Card*, QVector<Card*>> CombatCreatures){
    declareCombatCommand cmd = declareCombatCommand(state, CombatCreatures);
    if (cmd.isValid()){
        cmd.execute();
        emit updateUI();
    }
}

void gamemanager::onPlayerLost(){
    return;
}
// // Player object communication
