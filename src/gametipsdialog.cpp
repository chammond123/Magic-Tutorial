#include "gametipsdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

GameTipsDialog::GameTipsDialog(const QString gameTip, QWidget *parent)
    : QDialog(parent){

    setWindowTitle("Info");

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(gameTip, this);
    QPushButton *closeButton = new QPushButton("Ok", this);

    layout->addWidget(label);
    layout->addWidget(closeButton);

    label->setAlignment(Qt::AlignCenter);

    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}
