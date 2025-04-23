#include "manacollection.h"
#include "card.h"
#include "player.h"
#include "ui_manacollection.h"

ManaCollection::ManaCollection(Player* player, Card* card, QWidget *parent)
    : QDialog(parent), ui(new Ui::ManaCollection), player(player)
    , card(card)
{
    ui->setupUi(this);

    remainingMana = player->manaPool;

    // Set all Icons
    ui->redIcon->setPixmap(QPixmap(":/Icons/Icons/Red.png").scaled(
        ui->redIcon->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->blackIcon->setPixmap(QPixmap(":/Icons/Icons/Black.png").scaled(
        ui->redIcon->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->whiteIcon->setPixmap(QPixmap(":/Icons/Icons/white.png").scaled(
        ui->redIcon->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->greenIcon->setPixmap(QPixmap(":/Icons/Icons/Green.png").scaled(
        ui->redIcon->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->blueIcon->setPixmap(QPixmap(":/Icons/Icons/Blue.png").scaled(
        ui->redIcon->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Set payment from card
    ui->manaCost->setValue(card->cost[ManaType::ANY]);

    setRemainingMana();

    connect(ui->redPayment, &QSpinBox::valueChanged, this, &ManaCollection::updateMana);
    connect(ui->whitePayment, &QSpinBox::valueChanged, this, &ManaCollection::updateMana);
    connect(ui->bluePayment, &QSpinBox::valueChanged, this, &ManaCollection::updateMana);
    connect(ui->greenPayment, &QSpinBox::valueChanged, this, &ManaCollection::updateMana);
    connect(ui->blackPayment, &QSpinBox::valueChanged, this, &ManaCollection::updateMana);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

ManaCollection::~ManaCollection()
{
    delete ui;
}

void ManaCollection::setRemainingMana() {

    for (auto [color, value] : remainingMana.toStdMap()) {
        remainingMana[color] -= value;
    }

    updateMana();
}

void ManaCollection::updateMana() {
    int whiteCost = ui->whitePayment->value();
    int redCost = ui->redPayment->value();
    int blueCost = ui->bluePayment->value();
    int blackCost = ui->blackPayment->value();
    int greenCost = ui->greenPayment->value();

    manaPayments[ManaType::WHITE] = whiteCost;
    manaPayments[ManaType::RED] = redCost;
    manaPayments[ManaType::BLUE] = blueCost;
    manaPayments[ManaType::BLACK] = blackCost;
    manaPayments[ManaType::GREEN] = greenCost;

    int totalPayment = whiteCost + redCost + blueCost + blackCost + greenCost;
    int difference = card->cost[ManaType::ANY] - totalPayment;

    if(difference > 0){
        ui->buttonBox->setEnabled(false);
    }
    else {
        ui->buttonBox->setEnabled(true);
    }

    ui->manaCost->setValue(difference);

    ui->redMana->setValue(remainingMana[ManaType::RED] - redCost);
    ui->whiteMana->setValue(remainingMana[ManaType::WHITE] - whiteCost);
    ui->blueMana->setValue(remainingMana[ManaType::BLUE] - blueCost);
    ui->blackMana->setValue(remainingMana[ManaType::BLACK] - blackCost);
    ui->greenMana->setValue(remainingMana[ManaType::GREEN] - greenCost);
}

QMap<ManaType, int> ManaCollection::getUserMana(){
    return manaPayments;
}
