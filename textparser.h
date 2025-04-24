#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <iostream>

/**
 * @brief The TextParser class
 */
class TextParser
{
private:
    // Private constructor prevents instantiation
    TextParser() = delete;

public:
    static QStringList getListFromText(QFile input);
};

#endif // TEXTPARSER_H
