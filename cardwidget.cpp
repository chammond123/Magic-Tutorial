#include "cardwidget.h"
#include "ui_cardwidget.h"

CardWidget::CardWidget(const QString &name, const QString &cost, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CardWidget)
{
    setName(name);
    setCost(cost);
}

CardWidget::~CardWidget()
{
    delete ui;
}

void CardWidget::setName(const QString &name)
{
    ui->nameLabel->setText(name);
}

void CardWidget::setCost(const QString &cost)
{
    ui->costLabel->setText(cost);
}
