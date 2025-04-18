#include "cardbutton.h"
#include "carddictionary.h"
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsDropShadowEffect>

CardButton::CardButton(Card* card, QWidget* parent)
    : QPushButton(parent), cardPtr(card) {
    cardName = card->name;
    QPixmap pixmap = QPixmap::fromImage(card->image).scaled(
        this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setIcon(QIcon(pixmap));
    this->setText("");

    this->setStyleSheet(R"(
    QPushButton {
        border: 3px solid transparent;
        border-radius: 7px;
    }
)");

    this->setCheckable(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(this, &QPushButton::toggled, this, [this](bool checked) {
        if (checked) {
            auto* shadow = new QGraphicsDropShadowEffect(this);
            shadow->setBlurRadius(15);
            shadow->setColor(QColor(0, 0, 0));
            shadow->setOffset(0, 0);
            this->setGraphicsEffect(shadow);
        } else {
            this->setGraphicsEffect(nullptr);
        }
    });
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

void CardButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit cardSelected(this); // let MainWindow handle exclusive selection
    }
    // QPushButton::mousePressEvent(event);
}

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

void CardButton::resizeEvent(QResizeEvent* event) {
    QPushButton::resizeEvent(event); // Call base

    QPixmap pixmap = QPixmap::fromImage(cardPtr->image).scaled(
        this->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setIcon(QIcon(pixmap));
    this->setIconSize(this->size()- QSize(6,6));
}

