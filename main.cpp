#include "mainwindow.h"
#include "player.h"
#include "textparser.h"
#include "carddictionary.h"
#include "card.h"
#include "cardapimanager.h"

#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /**
     * Test Phases / Game States
     */

    CardAPIManager *apiManager = new CardAPIManager();
    qDebug() << "made it through API initialization.";
    // Connecting the API Manager's signal
    QObject::connect(apiManager, &CardAPIManager::cardFetched, [=](const Card &card) {
        cardDictionary::addCard(card);
        qDebug() << "Static Card Dictionary updated after a card was fetched";
    });

    qDebug() << "made it through API connection.";
    // Instantiating a deck list
    QStringList deckList = TextParser::getListFromText(QFile(":/text/additional_files/deck.txt"));

    // Fetching from the API
    apiManager->fetchCardsByNames(deckList);

    Player userPlayer(deckList);
    Player enemyPlayer(deckList);
    qDebug() << "made it through deck initialization.";
    GameState originalState;
    GameState secondState;

    originalState.player1 = &userPlayer;
    originalState.player2 = &enemyPlayer;

        // Crafted Gamestate
        // another crafted gamestate
        //MainWindow w(first crafted gamestate)

        // Connection for mainwindow signal to main.cpp lambda to change
        // MainWindows gamestate to the 2nd crafted gamestate
    qDebug() << "made it through object initialization.";
    MainWindow w(nullptr);

    // QObject::connect(w, some signal saying UI needs a new game state, signal to ui to update gamestate)
    w.show();
    qDebug() << "made it to window shown.";
    return a.exec();
}
