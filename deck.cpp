#include "deck.h"
#include "carddictionary.h"
#include <QFile>

Deck::Deck(QString fileName, QObject *parent)
    : QObject{parent}, fileName{fileName}
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);

    if (file.isOpen()){
        while(!file.atEnd()){
            QString cardName = file.readLine();
            Card card = cardDictionary::getCard(cardName);
            cards.append(card);
        }
    }

    file.close();
}

QVector<Card> Deck::getCardObjects() {
    return this->cards;
}
