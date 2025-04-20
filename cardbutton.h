#ifndef CARDBUTTON_H
#define CARDBUTTON_H

#include "card.h"
#include <QPushButton>

class CardButton : public QPushButton{
    Q_OBJECT
public:
    Card* cardPtr;
    QString cardName;
    bool tapped = false;

    explicit CardButton(Card* card, QWidget* parent = nullptr);

    QPixmap getOverlayedPixmap(int selectionIndex);
    void enableCard(bool enabled);

private:
    void updateTapped();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    // void mouseDoubleClickEvent(QMouseEvent* event) override;

signals:
    void cardSelected(CardButton* button);
    void hovered(Card* card);
    void cardTapped(CardButton* card, bool tapped);


public slots:
    void resetCard();
    void setTapped(bool tapped);
};




#endif // CARDBUTTON_H
