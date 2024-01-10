#include "QChromeTab.h"

using namespace VisualLinkerScript::Components::QChromeTab;


void QChromeTab::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);

    QPen pen(Qt::red);
    pen.setWidth(10);
    painter.setPen(pen);

    const qreal halfPenWidth = pen.width() / 2;

    painter.drawRect(QRectF(halfPenWidth, halfPenWidth,
        width() - halfPenWidth * 2, height() - halfPenWidth * 2));
}
