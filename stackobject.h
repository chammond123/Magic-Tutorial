#include "player.h"
#include <QVector>
#include <variant>
#ifndef STACKOBJECT_H
#define STACKOBJECT_H


struct StackObject{
    Player* player;
    Card* card;
    std::variant<Player*, Card*, std::nullptr_t> target;
};

#endif // STACKOBJECT_H
