#include "mainwindow.h"
#include "cardapimanager.h"
#include "ui_mainwindow.h"
#include "carddictionary.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CardAPIManager *apiManager = new CardAPIManager(this);

    connect(apiManager, &CardAPIManager::cardFetched,
            this, &carddictionary::addCard);

        apiManager->fetchCardByName("Lightning Bolt");
}

MainWindow::~MainWindow()
{
    delete ui;
}
