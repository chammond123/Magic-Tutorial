#include "gametipsdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

GameTipsDialog::GameTipsDialog(const QString gameTip, QWidget *parent)
    : QDialog(parent){

    setWindowTitle("TEST");

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(gameTip, this);
    QPushButton *closeButton = new QPushButton("Close Me!", this);

    layout->addWidget(label);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}
