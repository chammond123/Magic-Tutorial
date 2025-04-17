#include "mainwindow.h"
#include "player.h"
#include "textparser.h"
#include "carddictionary.h"
#include "card.h"
#include "cardapimanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /**
     * Test Phases / Game States
     */

    CardAPIManager *apiManager = new CardAPIManager();

    // Connecting the API Manager's signal
    QObject::connect(apiManager, &CardAPIManager::cardFetched, [=](const Card &card) {
        cardDictionary::addCard(card);
        qDebug() << "Static Card Dictionary updated after a card was fetched";
    });

    // Instantiating a deck list
    QStringList deckList = TextParser::getListFromText(QFile(":/text/additional_files/deck.txt"));

    // Fetching from the API
    apiManager->fetchCardsByNames(deckList);

    // Player userPlayer()

    MainWindow w(nullptr);
    w.show();
    return a.exec();
}
