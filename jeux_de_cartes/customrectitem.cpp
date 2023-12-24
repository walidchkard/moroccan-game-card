// customrectitem.cpp

#include "customrectitem.h"

CustomRectItem::CustomRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, width, height, parent)
{
    // Constructor implementation
}

void CustomRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Emit signal with row and column information based on the position of the item
    emit rectClicked(static_cast<int>(y() / rect().height()) + 1, static_cast<int>(x() / rect().width()));
}
