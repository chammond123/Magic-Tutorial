#include "tutorialpage.h"
#include "ui_tutorialpage.h"
#include <QPushButton>

TutorialPage::TutorialPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TutorialPage)
{
    ui->setupUi(this);
    setWindowTitle("Tutorial");

    connect(ui->backButton, &QPushButton::clicked, this, &TutorialPage::backToMenu);
}

TutorialPage::~TutorialPage() {
    delete ui;
}
