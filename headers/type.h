#ifndef TYPE_H
#define TYPE_H

#include <QString>

/**
 * @brief The CardType enum
 */
enum class
    CardType : int {
    LAND,
    CREATURE,
    ARTIFACT,
    ENCHANTMENT,
    PLANESWALKER,
    BATTLE,
    INSTANT,
    SORCERY
};

/**
 * @brief The PropertyType enum
 */
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

/**
 * @brief The ManaType enum
 */
enum class ManaType : int {
    WHITE,
    RED,
    BLUE,
    BLACK,
    GREEN,
    COLORLESS,
    ANY
};

/**
 * @brief The abilityType enum
 */
enum class abilityType : int {
    DAMAGE,
    DRAW,
    ADD_MANA,
    COUNTER,
    BYPASS_SUMMONING_SICKNESS
};

/**
 * @brief The ZoneType enum
 */
enum class ZoneType : int {
    LIBRARY,
    GRAVEYARD,
    BATTLEFIELD,
    EXILE,
    HAND,
    STACK
};

#endif // TYPE_H
