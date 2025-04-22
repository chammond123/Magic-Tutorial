#include "loadscreen.h"
#include "mainwindow.h"
#include "player.h"
#include "textparser.h"
#include "carddictionary.h"
#include "card.h"
#include "cardapimanager.h"
#include <QProgressDialog>
#include "bot.h"

#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /**
     * Test Phases / Game States
     */


    // Instantiating a deck list
    QStringList deckList = TextParser::getListFromText(QFile(":/text/additional_files/uniqueCards.txt"));
    QStringList playerDeckList = TextParser::getListFromText(QFile(":/text/additional_files/deck.txt"));

    loadScreen loadScreen;

    QObject::connect(&loadScreen, &loadScreen::loadingComplete, [&]() {

        Player *userPlayer = new Player(playerDeckList);
        userPlayer->isActivePlayer = true;
        userPlayer->holdingPriority = true;

        Player *enemyPlayer = new Player(playerDeckList);
        enemyPlayer->isActivePlayer = false;
        enemyPlayer->holdingPriority = false;

        qDebug() << "made it through deck initialization.";

        GameState *originalState = new GameState();
        // // GameState secondState;

        originalState->player1 = userPlayer;
        originalState->player2 = enemyPlayer;
        // Player userPlayer(playerDeckList);
        Bot bot(playerDeckList);

        // for testing
        // userPlayer.isActivePlayer = true;
        // userPlayer.holdingPriority = true;
        // Player enemyPlayer(playerDeckList);
        // for testing
        // enemyPlayer.isActivePlayer = false;
        // enemyPlayer.holdingPriority = false;
        // qDebug() << "made it through deck initialization.";
        // GameState originalState;
        // GameState secondState;

        // originalState.player1 = &userPlayer;
        // originalState.player2 = &bot;

        // Crafted Gamestate
        // another crafted gamestate
        //MainWindow w(first crafted gamestate)

        // Connection for mainwindow signal to main.cpp lambda to change
        // MainWindows gamestate to the 2nd crafted gamestate
        qDebug() << "made it through object initialization.";

        gamemanager* game = new gamemanager(nullptr);
        game->state = originalState;
        MainWindow *w = new MainWindow(game);

        w->show();
        qDebug() << "made it to window shown.";
    });

    // Fetching from the API
    // apiManager->fetchCardsByNames(deckList);
    loadScreen.show();
    loadScreen.startLoading(deckList);


    return a.exec();
}
