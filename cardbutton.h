#ifndef CARDBUTTON_H
#define CARDBUTTON_H

#include "card.h"
#include <QPushButton>

class CardButton : public QPushButton{
    Q_OBJECT
public:
    Card* cardPtr;
    QString cardName;
    bool selected = false;

    explicit CardButton(Card* card, QWidget* parent = nullptr);

    // void updateVisual();

    void setSelected(bool selected);
    bool isSelected();

signals:
    void cardSelected(CardButton* button);
    void hovered(Card* card);

protected:
    // void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;

public slots:
    void updateCard(const Card &card);
};


#endif // CARDBUTTON_H
