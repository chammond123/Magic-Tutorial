#include "textparser.h"
#include <sstream>

std::vector<std::string> TextParser::getListFromText(std::string filePath){
    std::vector<std::string> deckList;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return deckList;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t spaceIndex = line.find(" ");
        if (spaceIndex == std::string::npos) continue;

        // Number of cards
        int num = std::stoi(line.substr(0, spaceIndex));

        // Type of card
        std::string card = line.substr(spaceIndex + 1);

        for (int i = 0; i < num; i++) {
            deckList.push_back(card);
        }
    }

    file.close();
    return deckList;
}
