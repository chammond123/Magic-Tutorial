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

enum class PropertyType : int {
    AURA,
    DEATHTOUCH,
    DOUBLE_STRIKE,
    EQUIPMENT,
    FIRST_STRIKE,
    FLYING,
    HASTE,
    LIFELINK,
    REACH,
    TRAMPLE
};

enum class ManaType : int {
    WHITE,
    RED,
    BLUE,
    BLACK,
    GREEN,
    COLORLESS,
    ANY
};

enum class abilityType : int {
    DAMAGE,
    DRAW,
    ADD_MANA
};

enum class ZoneType : int {
    LIBRARY,
    GRAVEYARD,
    BATTLEFIELD,
    EXILE,
    HAND
};

#endif // TYPE_H
