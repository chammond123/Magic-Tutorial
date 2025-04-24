#ifndef TUTORIALPAGE_H
#define TUTORIALPAGE_H
#include <QVector>
#include <QString>
#include <QWidget>

namespace Ui {
class TutorialPage;
}


struct TutorialSlide {
    QString imagePath;
    QString text;
};

/**
 * @brief The TutorialPage class
 */
class TutorialPage : public QWidget {
    Q_OBJECT

public:
    explicit TutorialPage(QWidget *parent = nullptr);
    ~TutorialPage();

signals:
    void backToMenu();

private slots:
    void showNextSlide();
    void showPreviousSlide();

private:
    Ui::TutorialPage *ui;
    QVector<TutorialSlide> slides;
    int currentIndex = 0;
    void updateSlide();
};
#endif // TUTORIALPAGE_H
