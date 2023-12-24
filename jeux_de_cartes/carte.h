// carte.h
#ifndef CARTE_H
#define CARTE_H

#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QObject>
#include <QList>

class carte :public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    explicit carte(QGraphicsItem *parent = nullptr, int color = 0, int number = 0);
    void setCardProperties(int color, int number);
    int getColor() const { return color; }
    int getNumber() const { return number; }
    void showCardFront();
    void showCardBack();
    void setCurrentRect(int row, int col);
    QPoint getCurrentRect() const;
    void animateToPosition(const QPointF &endPos);
    QList<carte *> playerCollectedCards;

signals:
    void carteClicked(carte* clickedCarte);
    void movedToNewRect();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int color;
    int number;
    bool isFront;
    QPoint currentRect;
    bool hasBeenClicked;
    bool isAnimating = false;

};

#endif // CARTE_H
