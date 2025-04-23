#ifndef ABOUTPAGE_H
#define ABOUTPAGE_H

#include <QWidget>

namespace Ui {

/**
 * The about page has the authors credit as well as simply explaining the purpose and features of the game
 */
class AboutPage;
}

class AboutPage : public QWidget {
    Q_OBJECT

public:
    explicit AboutPage(QWidget *parent = nullptr);
    ~AboutPage();

signals:
    void backToMenu();

private:
    Ui::AboutPage *ui;
};

#endif // ABOUTPAGE_H
