// customrectitem.h

#ifndef CUSTOMRECTITEM_H
#define CUSTOMRECTITEM_H

#include <QGraphicsRectItem>

class CustomRectItem : public QGraphicsRectItem
{
    Q_OBJECT

public:
    CustomRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);

signals:
    void rectClicked(int row, int col);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // CUSTOMRECTITEM_H
