#include "lotusdialog.h"
#include "ui_lotusdialog.h"

LotusDialog::LotusDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LotusDialog)
{
    ui->setupUi(this);

    ui->red->setIcon(QIcon(QPixmap(":/Icons/Icons/Red.png").scaled(
        ui->red->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    ui->black->setIcon(QIcon(QPixmap(":/Icons/Icons/Black.png").scaled(
        ui->red->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    ui->white->setIcon(QIcon(QPixmap(":/Icons/Icons/white.png").scaled(
        ui->red->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    ui->green->setIcon(QIcon(QPixmap(":/Icons/Icons/Green.png").scaled(
        ui->red->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    ui->blue->setIcon(QIcon(QPixmap(":/Icons/Icons/Blue.png").scaled(
        ui->red->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation)));

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(ui->red, &QPushButton::clicked, this, &LotusDialog::on_redButton_clicked);
    connect(ui->green, &QPushButton::clicked, this, &LotusDialog::on_greenButton_clicked);
    connect(ui->blue, &QPushButton::clicked, this, &LotusDialog::on_blueButton_clicked);
    connect(ui->black, &QPushButton::clicked, this, &LotusDialog::on_blackButton_clicked);
    connect(ui->white, &QPushButton::clicked, this, &LotusDialog::on_whiteButton_clicked);
}

ManaType LotusDialog::getData() {
    return data;
}

void LotusDialog::on_redButton_clicked() {
    data = ManaType::RED;
}

void LotusDialog::on_greenButton_clicked() {
    data = ManaType::GREEN;
}

void LotusDialog::on_blueButton_clicked() {
    data = ManaType::BLUE;
}

void LotusDialog::on_blackButton_clicked() {
    data = ManaType::BLACK;
}

void LotusDialog::on_whiteButton_clicked() {
    data = ManaType::WHITE;
}

LotusDialog::~LotusDialog()
{
    delete ui;
}

