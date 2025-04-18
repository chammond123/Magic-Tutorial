#include "player.h"
#include <QVector>
#ifndef STACKOBJECT_H
#define STACKOBJECT_H

struct StackObject{
    Player* player;
    Card* card;
    Card* target;
};

#endif // STACKOBJECT_H
