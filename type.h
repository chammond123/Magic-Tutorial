#ifndef TYPE_H
#define TYPE_H

#include <QString>

enum class Type : int {
    LAND,
    CREATURE,
    ARTIFACT,
    ENCHANTMENT,
    PLANESWALKER,
    BATTLE,
    INSTANT,
    SORCERY
};

enum class ManaColor : int {
    Blue,
    Red,
    Green,
    White,
    Black,
};
#endif // TYPE_H
