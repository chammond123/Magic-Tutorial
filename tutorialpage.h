#ifndef TUTORIALPAGE_H
#define TUTORIALPAGE_H

#include <QWidget>

namespace Ui {
class TutorialPage;
}

class TutorialPage : public QWidget {
    Q_OBJECT

public:
    explicit TutorialPage(QWidget *parent = nullptr);
    ~TutorialPage();

signals:
    void backToMenu();

private:
    Ui::TutorialPage *ui;
};

#endif // TUTORIALPAGE_H
