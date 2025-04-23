#ifndef LOADSCREEN_H
#define LOADSCREEN_H

#include "card.h"
#include "cardapimanager.h"
#include "qprogressdialog.h"
#include <QDialog>

namespace Ui {
/**
 * This is a loading screen pop up when loading the all of the elements of the game
 */
class loadScreen;
}

class loadScreen : public QDialog
{
    Q_OBJECT

public:
    explicit loadScreen(QWidget *parent = nullptr);
    ~loadScreen();

    // Function to start loading cards
    void startLoading(const QStringList &cardList);

    // Get the loaded cards
    QList<Card> getLoadedCards() const;

signals:
    void loadingComplete();

private slots:
    void updateProgress(int value);
    void onLoadingFinished(const QList<Card> &cards);

private:
    Ui::loadScreen *ui;
    QProgressBar *progressBar;
    QLabel *statusLabel;
    CardAPIManager *apiManager;
    QList<Card> loadedCards;
};

#endif // LOADSCREEN_H
