#include "loadscreen.h"
#include "qboxlayout.h"
#include "carddictionary.h"
#include "qlabel.h"
#include <QProgressBar>
#include "qtimer.h"
#include "ui_loadscreen.h"

loadScreen::loadScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loadScreen)
{
    ui->setupUi(this);
    setWindowTitle("Loading Cards");
    setModal(true);
    setFixedSize(400, 150);

    QVBoxLayout *layout = new QVBoxLayout(this);

    statusLabel = new QLabel("Loading card database...");
    layout->addWidget(statusLabel);

    progressBar = new QProgressBar();
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    layout->addWidget(progressBar);

    // Initialize API manager
    apiManager = new CardAPIManager(this);

    // Connect signals/slots
    connect(apiManager, &CardAPIManager::batchFetchProgress,
            this, &loadScreen::updateProgress);
    connect(apiManager, &CardAPIManager::batchCardsFetched,
            this, &loadScreen::onLoadingFinished);
    connect(apiManager, &CardAPIManager::cardFetched,
            [](const Card &card) {
                cardDictionary::addCard(card);
                qDebug() << "Static Card Dictionary updated after a card was fetched";
            });
}

void loadScreen::startLoading(const QStringList &cardList) {
    if (cardList.isEmpty()) {
        emit loadingComplete();
        accept();
        return;
    }

    progressBar->setMaximum(cardList.size());
    apiManager->fetchCardsByNames(cardList);
}

void loadScreen::updateProgress(int value) {
    progressBar->setValue(value);
    statusLabel->setText(QString("Loading cards... %1/%2").arg(value).arg(progressBar->maximum()));
    // QApplication::processEvents();
}

void loadScreen::onLoadingFinished(const QList<Card> &cards) {
    loadedCards = cards;
    statusLabel->setText("Loading complete!");
    progressBar->setValue(progressBar->maximum());

    emit loadingComplete();

    // Close the dialog after a short delay
    QTimer::singleShot(500, this, &loadScreen::accept);
}

loadScreen::~loadScreen()
{
    delete ui;
}

