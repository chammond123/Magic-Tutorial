#include "textparser.h"

QStringList TextParser::getListFromText(QFile file){
    QStringList deckList;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return deckList;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        int spaceIndex = line.indexOf(" ");

        // Number of cards
        int num = line.mid(0, spaceIndex).toInt();

        // Type of card
        QString card = line.mid(spaceIndex + 1);

        for (int i = 0; i < num; i++) {
            deckList.append(card);
        }
    }

    file.close();
    return deckList;
}
