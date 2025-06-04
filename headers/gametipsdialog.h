#ifndef GAMETIPSDIALOG_H
#define GAMETIPSDIALOG_H

#include <QDialog>

class QLabel;
class QPushButton;

/**
 * @brief The GameTipsDialog class
 */
class GameTipsDialog : public QDialog
{
    Q_OBJECT
public:
    GameTipsDialog(const QString gameTip, QWidget *parent = nullptr);
};

#endif // GAMETIPSDIALOG_H
