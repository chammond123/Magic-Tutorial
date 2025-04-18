#include "cardbutton.h"
#include "carddictionary.h"
#include <QMouseEvent>
#include <QDebug>

CardButton::CardButton(Card* card, QWidget* parent)
    : QPushButton(parent), cardPtr(card) {
    cardName = card->name;
}

// void CardButton::updateVisual() {
//     if (cardPtr) {
//         setText(cardPtr->name);
//     }
// }

void CardButton::updateCard(const Card &card){
    if (card.name != cardName) return; // Confirm it's the correct card

    Card cardFromDictionary = cardDictionary::getCard(card.name);

    QPixmap pixmap = QPixmap::fromImage(cardFromDictionary.image).scaled(
        this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    this->setIcon(QIcon(pixmap));
    this->setText("");
    this->setIconSize(pixmap.rect().size());
}

// void CardButton::mousePressEvent(QMouseEvent* event) {
//     if (event->button() == Qt::LeftButton) {
//         emit cardSelected(this); // let MainWindow handle exclusive selection
//     }
//     QPushButton::mousePressEvent(event);
// }

void CardButton::setSelected(bool value) {
    if(value){
        selected = true;
        qDebug() << cardName << "selected";
        // qDebug() << cardPtr->description ;
    } else {
        selected = false;
        qDebug() << cardName << "deselected";
    }
}

void CardButton::enterEvent(QEnterEvent* event) {
    emit hovered(cardPtr);
    QPushButton::enterEvent(event);
}

