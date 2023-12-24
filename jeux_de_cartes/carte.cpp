// carte.cpp
#include "carte.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

carte::carte(QGraphicsItem *parent, int color, int number)
    : QObject(), QGraphicsPixmapItem(parent), color(color), number(number)
{
    showCardFront();
    setAcceptHoverEvents(true);
    hasBeenClicked = false;
}
void carte::animateToPosition(const QPointF &endPos)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setEndValue(endPos);
    animation->setDuration(500);
    animation->start();

    connect(animation, &QPropertyAnimation::finished, this, [this, animation]() {
        emit movedToNewRect();
        animation->deleteLater();
    });
}

void carte::setCardProperties(int color, int number)
{
    this->color = color;
    this->number = number;

    // Update the image based on the new color and number
    QString imagePath = QString(":/cartes/%1%2.gif").arg(color).arg(number);
    QPixmap cardImage(imagePath);
    setPixmap(cardImage.scaled(90, 135));
}

void carte::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Check if the card is in hand (dark green area)
    if (sceneBoundingRect().top() > 5 * 135) {
        // Get a list of empty rectangles in the target range (rows 2 to 4)
        QList<QGraphicsRectItem *> emptyRects;
        QList<QGraphicsItem *> sceneItems = scene()->items();

        for (QGraphicsItem *item : sceneItems) {
            if (item->type() == QGraphicsRectItem::Type && item != this) {
                QGraphicsRectItem *rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);

                // Check if the rectangle is within the desired height range
                qreal rectCenterY = rectItem->sceneBoundingRect().center().y();
                if (rectCenterY >= 1.5 * 135 && rectCenterY <= 4.5 * 135) {
                    // Check if the rectangle is empty
                    QList<QGraphicsItem *> collidingItems = rectItem->collidingItems();
                    bool isEmpty = std::all_of(collidingItems.begin(), collidingItems.end(), [](QGraphicsItem *item) {
                        return item->type() == QGraphicsRectItem::Type; // Only consider other rectangles
                    });

                    if (isEmpty) {
                        emptyRects.append(rectItem);
                    }
                }
            }
        }

        // Get a list of cards on the table (rows 2 to 4)
        QList<carte *> tableCards;

        for (QGraphicsItem *item : sceneItems) {
            // Check if the item is an instance of the carte class
            if (item->type() == QGraphicsPixmapItem::Type && dynamic_cast<carte *>(item)) {
                carte *tableCard = static_cast<carte *>(item);
                if (tableCard->getCurrentRect().y() >= 2 && tableCard->getCurrentRect().y() <= 4) {
                    tableCards.append(tableCard);
                }
            }
        }

        // Iterate through all the cards on the table and check for a match
        auto matchingCard = std::find_if(tableCards.begin(), tableCards.end(), [this](carte *tableCard) {
            return tableCard->getNumber() == this->getNumber();
        });

        // Connect the click signal to a slot or handle it directly
        connect(this, &carte::carteClicked, [this, tableCards, emptyRects, matchingCard]() {
            // If there is a matching card on the table, move it to the position of the matching card
            if (matchingCard != tableCards.end()) {
                carte *matchingTableCard = *matchingCard;
                QPointF rectCenter = matchingTableCard->sceneBoundingRect().center();
                QPointF newPos = rectCenter - QPointF(pixmap().width() / 2, pixmap().height() / 2);

                // Create a sequential animation group
                QSequentialAnimationGroup *animationGroup = new QSequentialAnimationGroup(this);
                playerCollectedCards.append(this);
                playerCollectedCards.append(matchingTableCard);

                // Animate the player's card to the matching card's position
                QPropertyAnimation *playerCardAnimation = new QPropertyAnimation(this, "pos");
                playerCardAnimation->setEndValue(newPos);
                playerCardAnimation->setDuration(700);
                playerCardAnimation->setEasingCurve(QEasingCurve::InOutQuad);

                // Connect the animation finished signal to a slot or lambda
                connect(animationGroup, &QSequentialAnimationGroup::finished, this, [this, matchingTableCard]() {
                    // Disconnect the player's card from the player's click signal
                    disconnect(this, &carte::carteClicked, nullptr, nullptr);
                    // Connect the player's card's click signal to a slot or handle it directly
                    connect(this, &carte::carteClicked, []() {
                        qDebug() << "Player's Card Clicked!";
                        // Handle the player's card click event if needed
                    });
                    // Remove the cards from the scene
                    matchingTableCard->scene()->removeItem(matchingTableCard);
                });

                // Connect the animation finished signal to a slot or lambda
                connect(animationGroup, &QSequentialAnimationGroup::finished, this, [this]() {
                    // Disconnect the player's card from the player's click signal
                    disconnect(this, &carte::carteClicked, nullptr, nullptr);
                    // Connect the player's card's click signal to a slot or handle it directly
                    connect(this, &carte::carteClicked, []() {
                        qDebug() << "Player's Card Clicked!";
                        // Handle the player's card click event if needed
                    });
                    // Remove the cards from the scene
                    this->scene()->removeItem(this);
                });

                // Add animations to the group
                animationGroup->addAnimation(playerCardAnimation);

                // Start the animation group
                animationGroup->start();
                // Exit the function after starting animations
                return;
            }

            // If there is no matching card on the table, move to a random empty rectangle
            if (!emptyRects.isEmpty()) {
                QGraphicsRectItem *randomRect = emptyRects.at(QRandomGenerator::global()->bounded(emptyRects.size()));
                QPointF rectCenter = randomRect->sceneBoundingRect().center();
                QPointF newPos = rectCenter - QPointF(pixmap().width() / 2, pixmap().height() / 2);

                // Animate the card's position
                animateToPosition(newPos);

                // Update the current rectangle of the card
                setCurrentRect(randomRect->y() / 135 + 1, randomRect->x() / 90 + 2);

                // Show the front of the card
                showCardFront();
            }
        });
    }

    // Emit the signal that the card was clicked
    if (!hasBeenClicked) {
        hasBeenClicked = true;
        emit carteClicked(this);
    }

    // Call the base class implementation
    QGraphicsPixmapItem::mousePressEvent(event);
}


void carte::showCardFront()
{
    isFront = true;
    QString imagePath = QString(":/cartes/%1%2.gif").arg(color).arg(number);
    QPixmap cardImage(imagePath);
    setPixmap(cardImage.scaled(90, 135));
}

void carte::showCardBack()
{
    isFront = false;
    QPixmap cardImage(":/cartes/back.gif");
    setPixmap(cardImage.scaled(90, 135));
}

void carte::setCurrentRect(int row, int col)
{
    currentRect = QPoint(row, col);
}

QPoint carte::getCurrentRect() const
{
    return currentRect;
}
