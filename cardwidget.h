#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget {
    Q_OBJECT

public:
    explicit CardWidget(const QString &name, const QString &cost, QWidget *parent = nullptr);
    ~CardWidget();

    void setName(const QString &name);
    void setCost(const QString &cost);

private:
    Ui::CardWidget *ui;
};

#endif // CARDWIDGET_H
