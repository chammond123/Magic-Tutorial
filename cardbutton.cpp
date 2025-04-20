#include "cardbutton.h"
#include "carddictionary.h"
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>

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
        padding: 3px;
    }
    QPushButton:checked {
        border: 3px solid blue;
        border-radius: 5px;
    }
)");

    this->setCheckable(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // connect(this, &QPushButton::toggled, this, [this](bool checked) {
    //     if (checked) {
    //         auto* shadow = new QGraphicsDropShadowEffect(this);
    //         shadow->setBlurRadius(15);
    //         shadow->setColor(QColor(0, 0, 0));
    //         shadow->setOffset(0, 0);
    //         this->setGraphicsEffect(shadow);
    //     } else {
    //         this->setGraphicsEffect(nullptr);
    //     }
    // });
}



void CardButton::resetCard(){

    QPixmap pixmap = QPixmap::fromImage(cardPtr->image).scaled(
        this->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    this->setIcon(QIcon(pixmap));
    this->setText("");
    this->setIconSize(this->size() - QSize(6,6));
}

void CardButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit cardSelected(this); // let MainWindow handle exclusive selection
    }
    // QPushButton::mousePressEvent(event);
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

QPixmap CardButton::getOverlayedPixmap(int selectionIndex) {
    QPixmap original = QPixmap::fromImage(cardPtr->image).scaled(
        this->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPixmap overlayed = original;
    QPainter painter(&overlayed);

    // Step 1: Darken it
    painter.fillRect(overlayed.rect(), QColor(0, 0, 0, 60));  // semi-transparent black

    // Step 2: Draw number
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(24); // Adjust size to taste
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(overlayed.rect(), Qt::AlignCenter, QString::number(selectionIndex + 1));

    painter.end();
    return overlayed;
}

void CardButton::enableCard(bool enabled){
    if(enabled){
        resetCard();
    }
    else{
        QPixmap original = QPixmap::fromImage(cardPtr->image).scaled(
            this->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QPixmap overlayed = original;
        QPainter painter(&overlayed);

        painter.fillRect(overlayed.rect(), QColor(0, 0, 0, 120));

        painter.end();
        this->setIcon(overlayed);
    }

    QWidget::setEnabled(enabled);

}
