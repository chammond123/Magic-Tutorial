#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

namespace Ui {

/**
 * The main menu of the game
 */
class MainMenu;
}

class MainMenu : public QWidget {
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

signals:
    void startGame();
    void openAboutPage();
    void openTutorialPage();

private:
    Ui::MainMenu *ui;
};

#endif // MAINMENU_H
