#ifndef TUTORIALPAGE_H
#define TUTORIALPAGE_H

#include <QWidget>

namespace Ui {
class TutorialPage;
}

#include <QVector>
#include <QString>

struct TutorialSlide {
    QString imagePath;
    QString text;
};

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
