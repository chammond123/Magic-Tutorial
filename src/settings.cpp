#include "settings.h"
#include "ui_settings.h"
#include <QPushButton>
#include <QKeyEvent>
#include <QDialog>

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    setWindowTitle("Settings");

    connect(ui->menuButton, &QPushButton::clicked, this, [this]() {
        emit returnToMainMenu();
        close();
    });

    connect(ui->closeButton, &QPushButton::clicked, this, &Settings::close);
}

void Settings::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QDialog::keyPressEvent(event);
    }
}

Settings::~Settings()
{
    delete ui;
}
