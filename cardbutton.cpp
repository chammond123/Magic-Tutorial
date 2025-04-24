#include "cardbutton.h"
#include "carddictionary.h"
#include "qgraphicstransform.h"
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QGraphicsEffect>
#include <QPainterPath>

CardButton::CardButton(Card* card, QWidget* parent)
    : QPushButton(parent), cardPtr(card) {
    cardName = card->name;

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
    QImage baseImage = card->image;

    if (card->isTapped) {
        // Create a darkened copy of the image
        QImage darkened = baseImage;
        for (int y = 0; y < darkened.height(); ++y) {
            for (int x = 0; x < darkened.width(); ++x) {
                QColor color = darkened.pixelColor(x, y);
                color = color.darker(180);  // 80% dark
                darkened.setPixelColor(x, y, color);
            }
        }
        baseImage = darkened;
    }

    QPixmap pixmap = QPixmap::fromImage(baseImage).scaled(this->size() - QSize(6, 6),
                                                          Qt::KeepAspectRatio,
                                                          Qt::SmoothTransformation);

    this->setIcon(QIcon(pixmap));
    this->setText("");

    this->setCheckable(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}



void CardButton::resetCard(){

    if (!cardPtr) {
        qDebug() << "Warning: Attempt to reset card with null cardPtr";
        return;
    }

    if (cardPtr->image.isNull()) {
        qDebug() << "Warning: Card image is null for" << cardPtr->name;
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(cardPtr->image).scaled(
        this->size() - QSize(6,6), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // QTransform transform;

    // if(cardPtr->isTapped) {
    //     transform.rotate(90);
    //     setFixedSize(140, 100);
    // }
    // else {
    //     transform.rotate(0);
    //     setFixedSize(100, 140);
    // }
    // QPixmap rotated = pixmap.transformed(transform, Qt::SmoothTransformation);

    this->setIcon(QIcon(pixmap));
    this->setText("");
    this->setIconSize(this->size() - QSize(6,6));
}

void CardButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit cardSelected(this); // let MainWindow handle exclusive selection
        qDebug() << "left click called";
    }
    else if (event->button() == Qt::RightButton){

        emit cardTapped();
        qDebug() << "right click called";
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


    painter.fillRect(overlayed.rect(), QColor(0, 0, 0, 60));  // semi-transparent black


    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(24);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(overlayed.rect(), Qt::AlignCenter, QString::number(selectionIndex + 1));

    painter.end();
    return overlayed;
}

void CardButton::enableCard(bool enabled){

    if (!cardPtr) {
        qDebug() << "Warning: Attempt to enable/disable card with null cardPtr";
        QWidget::setEnabled(enabled);
        return;
    }

    // Check if image is valid
    if (cardPtr->image.isNull()) {
        qDebug() << "Warning: Card image is null for" << cardPtr->name;
        QWidget::setEnabled(enabled);
        return;
    }

    if(enabled){
        resetCard();
        //        this->setStyleSheet(R"(
        // 	QPushButton {
        //        	border: 3px solid yellow;
        //        	border-radius: 7px;
        // 	}
        // )");
    } else {
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
