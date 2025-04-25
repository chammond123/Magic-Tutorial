#ifndef CARDBUTTON_H
#define CARDBUTTON_H

#include "card.h"
#include <QPushButton>

/**
 * @brief The CardButton class represent the card as a QPushButton
 */
class CardButton : public QPushButton{
    Q_OBJECT
public:
    Card* cardPtr;
    QString cardName;
    bool isBack = false;
    bool allowHover = true;

    explicit CardButton(Card* card, QWidget* parent = nullptr);

    QPixmap getOverlayedPixmap(int selectionIndex, QColor color);
    void backCard();
    void enableCard(bool enabled);

private:


protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

signals:
    void cardSelected(CardButton* button);
    void hovered(Card* card);
    void cardTapped(CardButton* button);


public slots:
    void resetCard();
};


#endif // CARDBUTTON_H
