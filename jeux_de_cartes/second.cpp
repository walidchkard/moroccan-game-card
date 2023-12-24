#include "second.h"
#include "mainwindow.h"
#include "ui_second.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPushButton>
#include <QDebug>
#include <QScreen>
#include <QMouseEvent>
#include "carte.h"
#include <QTimer>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QUrl>

second::second(MainWindow *w, QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::second),
    w(w),
    deck(this)
{
    ui->setupUi(this);
    QPushButton *myButton3 = findChild<QPushButton*>("bt3");


    if (myButton3) {
        myButton3->setStyleSheet("QPushButton {"
                                 "background-color: #FFFF00;"
                                 "border-radius: 10px; "
                                 "border: 1.5px solid rgb(91,231,255); "
                                 "color: #FF0000;"
                                 "text-align: center;"
                                 "text-decoration: none;"
                                 "font-size: 16px;"
                                 "margin-left: 0px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "background-color: #FFA07A;"
                                 "color: black;"
                                 "font-size: 16px;"
                                 "}");
    }

    // Get the primary screen
    QScreen *screen = QGuiApplication::primaryScreen();

    // Set the size of the QGraphicsView to be the size of the screen
    this->setFixedSize(screen->size());

    scene = new QGraphicsScene(this);
    QPixmap bkgnd(":/background2.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    scene->setBackgroundBrush(bkgnd);
    // Draw transparent rectangles forming the reference grid
    int rectWidth = width() / 10;
    int rectHeight = height() / 5;

    for (int row = 1; row < 6; ++row) {
        for (int col = 2; col < 8; ++col) {
            QRectF rect(col * rectWidth, (row - 1) * rectHeight, rectWidth, rectHeight);
            QGraphicsRectItem *rectItem = new QGraphicsRectItem(rect);
            rectItem->setBrush(Qt::NoBrush); // Transparent brush
            scene->addItem(rectItem);
            rectItem->setPen(Qt::NoPen);
            buttons.append(rectItem);
        }
    }

    // Set the scene to the view
    setScene(scene);

    // Adjust the rendering hints and scaling factor as needed
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
    setRenderHint(QPainter::TextAntialiasing, true);
    setViewportUpdateMode(FullViewportUpdate);

    deckPixmap.load(":/cartes/back.gif");
    setDeckImage();

    M_player.setSource(QUrl("qrc:/sounds/M_player.mp3"));
    QAudioOutput* audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.3);
    M_player.setAudioOutput(audioOutput);
    M_player.play();

    connect(&autoAddCardsTimer, &QTimer::timeout, this, &second::checkAndPerformActions);
    autoAddCardsTimer.start(1000);
}

second::~second()
{
    delete ui;
}


void second::checkAndPerformActions()
{
    if (areRectsEmpty(1, 3) && areRectsEmpty(1, 4) && areRectsEmpty(1, 5) && areRectsEmpty(1, 6) &&
        areRectsEmpty(5, 3) && areRectsEmpty(5, 4) && areRectsEmpty(5, 5) && areRectsEmpty(5, 6)) {

        // Add cards to each rectangle
        addCarteToRectFromDeck(1, 3, 0);
        addCarteToRectFromDeck(1, 4, 0);
        addCarteToRectFromDeck(1, 5, 0);
        addCarteToRectFromDeck(1, 6, 0);
        addCarteToRectFromDeck(5, 3, 1);
        addCarteToRectFromDeck(5, 4, 1);
        addCarteToRectFromDeck(5, 5, 1);
        addCarteToRectFromDeck(5, 6, 1);
    }
    checkGameResult();

}


void second::on_bt3_clicked()
{
    this->hide();
    if (w) {
        w->show();
        scene->update();
    }
}
void second::addCarteToRectFromDeck(int row, int col, bool showFront)
{
    carte* card = deck.dealCard();
    if (card) {
        int index = (row - 1) * 6 + (col - 2);
        if (index >= 0 && index < buttons.size()) {
            QGraphicsRectItem *rectItem = buttons.at(index);
            QPointF rectCenter = rectItem->sceneBoundingRect().center();

            // Set the card properties using the accessor methods
            card->setCardProperties(card->getColor(), card->getNumber());

            // Position Carte with its center aligned to the center of the rectangle
            QPointF cardPos = rectCenter - QPointF(card->pixmap().width() / 2, card->pixmap().height() / 2);
            card->setPos(cardPos);

            // Show the front or back based on the argument
            if (showFront) {
                card->showCardFront();
            } else {
                card->showCardBack();
            }

            // Add the card to the scene
            scene->addItem(card);

            // Add the card to the list of added cards
            addedCards.append(card);

            // Connect the card's carteClicked signal to a slot or handle it directly
            connect(card, &carte::carteClicked, this, [this, card]() {
                emit carteClicked(card);
                QTimer::singleShot(1000, this, [this]() {
                    computerMove();
                });
            });
        }
    }
    setDeckImage();
}

void second::computerMove()
{
    // Get a list of player's cards on the table (row 1)
    QList<carte *> playerCards;
    for (int col = 2; col < 8; ++col) {
        int index = (1 - 1) * 6 + (col - 2); // Row 1
        QGraphicsRectItem *playerRectItem = buttons.at(index);

        // Check if the player's rectangle has a card
        QList<QGraphicsItem *> collidingItems = playerRectItem->collidingItems();
        for (QGraphicsItem *item : collidingItems) {
            if (item->type() == QGraphicsPixmapItem::Type) {
                playerCards.append(qgraphicsitem_cast<carte *>(item));
            }
        }
    }

    // Get a list of cards on the table (rows 2 to 4)
    QList<carte *> tableCards;
    for (int row = 2; row <= 4; ++row) {
        for (int col = 2; col < 8; ++col) {
            int index = (row - 1) * 6 + (col - 2);
            QGraphicsRectItem *tableRectItem = buttons.at(index);

            // Check if the table rectangle has a card
            QList<QGraphicsItem *> collidingItems = tableRectItem->collidingItems();
            for (QGraphicsItem *item : collidingItems) {
                if (item->type() == QGraphicsPixmapItem::Type) {
                    tableCards.append(qgraphicsitem_cast<carte *>(item));
                }
            }
        }
    }

    // Check if the player has any cards on the table
    if (!playerCards.isEmpty()) {
        // Iterate through the table cards and check for a match
        for (carte *tableCard : tableCards) {
            int tableCardNumber = tableCard->getNumber();

            // Find a matching card in the player's hand
            auto matchingCard = std::find_if(playerCards.begin(), playerCards.end(), [tableCardNumber](carte *playerCard) {
                return playerCard->getNumber() == tableCardNumber;
            });

            // If a matching card is found, move it to the position of the matching card on the table
            if (matchingCard != playerCards.end()) {
                carte *selectedPlayerCard = *matchingCard;
                QPointF rectCenter = tableCard->sceneBoundingRect().center();
                QPointF newPos = rectCenter - QPointF(selectedPlayerCard->pixmap().width() / 2, selectedPlayerCard->pixmap().height() / 2);

                // Create a sequential animation group
                QSequentialAnimationGroup *animationGroup = new QSequentialAnimationGroup(this);
                selectedPlayerCard->showCardFront();

                // Animate the player's card to the matching card's position
                QPropertyAnimation *playerCardAnimation = new QPropertyAnimation(selectedPlayerCard, "pos");
                playerCardAnimation->setEndValue(newPos);
                playerCardAnimation->setDuration(700);
                playerCardAnimation->setEasingCurve(QEasingCurve::InOutQuad);

                // Connect the animation finished signal to a slot or lambda
                connect(animationGroup, &QSequentialAnimationGroup::finished, this, [this, selectedPlayerCard, tableCard, animationGroup]() {
                    // Move the cards to the computer's collected cards list
                    computerCollectedCards.append(selectedPlayerCard);
                    computerCollectedCards.append(tableCard);

                    // Remove the cards from the scene
                    scene->removeItem(selectedPlayerCard);
                    scene->removeItem(tableCard);

                    // Disconnect the player's card from the player's click signal
                    disconnect(selectedPlayerCard, &carte::carteClicked, this, nullptr);

                    // Connect the player's card's click signal to a slot or handle it directly
                    connect(selectedPlayerCard, &carte::carteClicked, this, [this, selectedPlayerCard]() {
                        qDebug() << "Player's Card Clicked!";
                    });

                    // Clean up the animation group
                    animationGroup->deleteLater();
                });

                // Add animations to the group
                animationGroup->addAnimation(playerCardAnimation);

                // Start the animation group
                animationGroup->start();

                // Exit the function after starting animations
                return;
            }
        }
    }

    // If no matching card is found, move a random card from the player's hand to an empty slot on the table
    carte *randomPlayerCard = playerCards.at(QRandomGenerator::global()->bounded(playerCards.size()));
    QGraphicsRectItem *emptyTableRect = findEmptyTableRect();
    if (emptyTableRect) {
        QPointF rectCenter = emptyTableRect->sceneBoundingRect().center();
        QPointF newPos = rectCenter - QPointF(randomPlayerCard->pixmap().width() / 2, randomPlayerCard->pixmap().height() / 2);

        // Use the animateToPosition function to move the card with animation
        randomPlayerCard->animateToPosition(newPos);

        randomPlayerCard->setCurrentRect(emptyTableRect->y() / 135 + 1, emptyTableRect->x() / 90 + 2);

        randomPlayerCard->showCardFront();

        // Disconnect the player's card from the player's click signal
        disconnect(randomPlayerCard, &carte::carteClicked, this, nullptr);

        // Connect the player's card's click signal to a slot or handle it directly
        connect(randomPlayerCard, &carte::carteClicked, this, [this, randomPlayerCard]() {
            qDebug() << "Player's Card Clicked!";
        });
    }
    checkGameResult();
}
void second::showMessage(const QString &message)
{
    // Update the label to display the message
    ui->result->setText(message);
}
void second::checkGameResult()
{
    if (deck.isEmpty() && areRectsEmpty(1, 3) && areRectsEmpty(1, 4) && areRectsEmpty(1, 5) &&
        areRectsEmpty(1, 6) && areRectsEmpty(5, 3) && areRectsEmpty(5, 4) && areRectsEmpty(5, 5) &&
        areRectsEmpty(5, 6)) {
        if (computerCollectedCards.size() < 20) {
            showMessage("You won!");
            ui->result->setStyleSheet("color: green; font-size: 110px;");
        } else if (computerCollectedCards.size() == 20) {
            showMessage("DRAW!");
            ui->result->setStyleSheet("color: yellow; font-size: 110px;");
        } else if(computerCollectedCards.size()>20){
            showMessage("You lost!");
            ui->result->setStyleSheet("color: red; font-size: 110px;");
        }
        ui->result->setAlignment(Qt::AlignCenter);
    }
}

QGraphicsRectItem *second::findEmptyTableRect()
{
    // Get a list of empty rectangles on the table (rows 2 to 4)
    QList<QGraphicsRectItem *> emptyTableRects;
    for (int row = 2; row <= 4; ++row) {
        for (int col = 2; col < 8; ++col) {
            int index = (row - 1) * 6 + (col - 2);
            QGraphicsRectItem *tableRectItem = buttons.at(index);

            // Check if the table rectangle is empty
            QList<QGraphicsItem *> collidingItems = tableRectItem->collidingItems();
            bool isEmpty = std::all_of(collidingItems.begin(), collidingItems.end(), [](QGraphicsItem *item) {
                return item->type() == QGraphicsRectItem::Type; // Only consider other rectangles
            });

            if (isEmpty) {
                emptyTableRects.append(tableRectItem);
            }
        }
    }

    // Check if there are empty slots on the table
    if (!emptyTableRects.isEmpty()) {
        // Get a random empty table rectangle
        return emptyTableRects.at(QRandomGenerator::global()->bounded(emptyTableRects.size()));
    }

    return nullptr;
}

void second::setDeckImage()
{
    if (deck.isEmpty()) {
        // Deck is empty, set a different image
        ui->deckImageLabel->setPixmap(QPixmap(":/cartes/blank.gif"));
    } else {
        // Deck is not empty, set the default deck image
        ui->deckImageLabel->setPixmap(deckPixmap);
    }
}

bool second::areRectsEmpty(int row, int col)
{
    int index = (row - 1) * 6 + (col - 2);
    if (index >= 0 && index < buttons.size()) {
        QGraphicsRectItem *rectItem = buttons.at(index);

        // Check if the rectangle is empty
        QList<QGraphicsItem *> collidingItems = rectItem->collidingItems();
        return std::all_of(collidingItems.begin(), collidingItems.end(), [](QGraphicsItem *item) {
            return item->type() == QGraphicsRectItem::Type; // Only consider other rectangles
        });
    }

    return false;
}
