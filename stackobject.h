#include "player.h"
#include <QVector>
#ifndef STACKOBJECT_H
#define STACKOBJECT_H

struct StackObject{
    Player* player;
    Card* card;
    QVector<Card*>* targets;
};

#endif // STACKOBJECT_H
