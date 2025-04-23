#include "aboutpage.h"
#include "ui_aboutpage.h"
#include "QPushButton"

AboutPage::AboutPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutPage)
{
    ui->setupUi(this);
    setWindowTitle("About");

    ui->textBrowser->setReadOnly(true);

    connect(ui->backButton, &QPushButton::clicked, this, &AboutPage::backToMenu);
}

AboutPage::~AboutPage() {
    delete ui;
}
