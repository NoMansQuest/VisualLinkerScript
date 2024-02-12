#include "QChromeTabWidget.h"

using namespace VisualLinkerScript::Components;

void DrawTab(QPainter& painter);

void QChromeTabWidget::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);

    const float TextHeightMillimeters = 5.0f;
    const float TabHeightMillimeters = 9.0f;
    const float TabActiveBarHeightMillimeters = 1.0f;

    // NOTE:
    // Tab height is expected to be 9mm
    // Font height (text vertically centered) is expected to be 5mm

    // The height of the tab should be 8mm.
    auto dpmmX = painter.device()->logicalDpiX() / 25.4f;
    auto dpmmY = painter.device()->logicalDpiY() / 25.4f;
    auto tabHeightInPixels = dpmmY * 9;

#ifdef Q_OS_WIN
    QString fontFamilyName = "Tahoma";
#elif Q_OS_LINUX
    QString fontFamilyName = "DejaVu Sans";
#elif Q_OS_OSX
    QString fontFamilyName = "Helvetica Neue";
#endif

    auto defaultFont = QFont(fontFamilyName, (int)(5 * dpmmY));


    QPen pen(Qt::red);
    pen.setWidth(10);
    painter.setPen(pen);

    const qreal halfPenWidth = pen.width() / 2;

    painter.drawRect(QRectF(halfPenWidth, halfPenWidth,
        width() - halfPenWidth * 2, height() - halfPenWidth * 2));
}

void QChromeTabWidget::RecalculateGraphicCoordinates()
{

}

void DrawTab(QPainter& painter)
{

}
