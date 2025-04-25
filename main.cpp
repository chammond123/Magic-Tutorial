#include "loadscreen.h"
#include "mainwindow.h"
#include "player.h"
#include "textparser.h"
#include "carddictionary.h"
#include "card.h"
#include "cardapimanager.h"
#include "aboutpage.h"
#include <QProgressDialog>
#include "bot.h"
#include "mainmenu.h"
#include <QDebug>
#include <QApplication>
#include "tutorialpage.h"
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Instantiating a deck list
    QStringList deckList = TextParser::getListFromText(QFile(":/text/additional_files/uniqueCards.txt"));
    QStringList playerDeckList = TextParser::getListFromText(QFile(":/text/additional_files/deck.txt"));

    loadScreen loadScreen;
    MainMenu* menu = new MainMenu();
    AboutPage* about = new AboutPage();
    TutorialPage* tutorial = new TutorialPage();

    QObject::connect(menu, &MainMenu::openAboutPage, [&]() {
        about->show();
        menu->hide();
    });

    QObject::connect(about, &AboutPage::backToMenu, [&]() {
        about->hide();
        menu->show();
    });

    QObject::connect(&loadScreen, &loadScreen::loadingComplete, [&]() {
        menu->show();
    });

    QObject::connect(menu, &MainMenu::openTutorialPage, [&]() {
        tutorial->show();
        menu->hide();
    });

    QObject::connect(tutorial, &TutorialPage::backToMenu, [&]() {
        tutorial->hide();
        menu->show();
    });

    QObject::connect(menu, &MainMenu::startGame, [&]() {
        Player *userPlayer = new Player(playerDeckList);
        userPlayer->isActivePlayer = true;
        userPlayer->holdingPriority = true;

        Player *enemyPlayer = new Player(playerDeckList);
        enemyPlayer->isActivePlayer = false;
        enemyPlayer->holdingPriority = false;

        GameState *originalState = new GameState();
        originalState->player1 = userPlayer;
        originalState->player2 = enemyPlayer;

        Bot bot(playerDeckList);

        gamemanager* game = new gamemanager(nullptr);
        game->state = originalState;

        MainWindow *w = new MainWindow(game);

        QObject::connect(w, &MainWindow::returnToMainMenu, [=]() {
            w->close();
            menu->show();
        });

        w->show();
        menu->close();
    });

    loadScreen.show();
    loadScreen.startLoading(deckList);

    return a.exec();
}
