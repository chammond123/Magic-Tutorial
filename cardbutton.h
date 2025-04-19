#ifndef CARDBUTTON_H
#define CARDBUTTON_H

#include "card.h"
#include <QPushButton>

class CardButton : public QPushButton{
    Q_OBJECT
public:
    Card* cardPtr;
    QString cardName;

    explicit CardButton(Card* card, QWidget* parent = nullptr);

    QPixmap getOverlayedPixmap(int selectionIndex);

signals:
    void cardSelected(CardButton* button);
    void hovered(Card* card);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

public slots:
    void resetCard();
};


#endif // CARDBUTTON_H
