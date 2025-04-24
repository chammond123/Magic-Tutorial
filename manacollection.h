#ifndef MANACOLLECTION_H
#define MANACOLLECTION_H

#include "card.h"
#include "player.h"
#include <QDialog>

namespace Ui {
class ManaCollection;
}

/**
 * @brief The ManaCollection class
 */
class ManaCollection : public QDialog
{
    Q_OBJECT

public:
    explicit ManaCollection(Player* player, Card* card, QWidget *parent = nullptr);
    ~ManaCollection();
    QMap<ManaType, int> manaPayments;
    QMap<ManaType, int> getUserMana();


private:
    Ui::ManaCollection *ui;
    Player* player;
    Card* card;

    QMap<ManaType, int> remainingMana;

    void setRemainingMana();
    void updateMana();
};

#endif // MANACOLLECTION_H
