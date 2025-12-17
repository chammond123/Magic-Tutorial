#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <vector>
#include <string>
#include <fstream>
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
    static std::vector<std::string> getListFromText(std::string filePath);
};

#endif // TEXTPARSER_H
