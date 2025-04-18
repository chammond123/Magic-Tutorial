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

    cardDictionary::addCard(card);
    Card elfCard = cardDictionary::getCard(card.name);

    QPixmap pixmap = QPixmap::fromImage(elfCard.image).scaled(
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
    } else {
        selected = false;
        qDebug() << cardName << "deselected";
    }
}

