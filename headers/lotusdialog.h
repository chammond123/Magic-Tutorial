#ifndef LOTUSDIALOG_H
#define LOTUSDIALOG_H

#include <QDialog>
#include "type.h"

namespace Ui {
class LotusDialog;
}

class LotusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LotusDialog(QWidget *parent = nullptr);
    ~LotusDialog();
    ManaType data;
    ManaType getData();

public slots:
    void on_redButton_clicked();
    void on_greenButton_clicked();
    void on_blueButton_clicked();
    void on_blackButton_clicked();
    void on_whiteButton_clicked();

private:
    Ui::LotusDialog *ui;
};

#endif // LOTUSDIALOG_H
