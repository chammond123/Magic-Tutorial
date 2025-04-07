#ifndef TYPE_H
#define TYPE_H

#include <QString>

enum class CardType : int {
    LAND,
    CREATURE,
    ARTIFACT,
    ENCHANTMENT,
    PLANESWALKER,
    BATTLE,
    INSTANT,
    SORCERY
};

enum class ManaType : int {
    WHITE,
    RED,
    BLUE,
    BLACK,
    GREEN,
    ANY
};
#endif // TYPE_H
