#include "QMemoryLayoutRender.h"
#include <qguiapplication.h>
#include <QPainter>


// Helper functions
namespace
{
	constexpr QColor colorLevelM1(0x10, 0x10, 0x10);
	constexpr QColor colorGrid(0x20, 0x20, 0x20);
    constexpr QColor colorBorder(0x30, 0x30, 0x30);

    qreal getFontSizeFromMetric(const QWidget* targetWidget, const double desiredHeightMilliMeters)
    {
        const auto widgetPosition = targetWidget->mapToGlobal(QPoint(0, 0));
        const auto screen = QGuiApplication::screenAt(widgetPosition);
        const double dpi = screen->logicalDotsPerInch();
        const double heightInInches = desiredHeightMilliMeters / 25.4; // 1 inch = 25.4 mm
        const double heightInPixels = heightInInches * dpi;
        return heightInPixels * 72 / dpi;
    }

    qreal getPixelsInMetric(const QWidget* targetWidget, const double desiredSizeInMilliMeters)
    {
        if (!targetWidget->screen()) {
            qWarning("No screen provided!");
            return 0.0;
        }

        double dpi = targetWidget->screen()->physicalDotsPerInch();
        return (dpi / 25.4) * desiredSizeInMilliMeters;
    }
};

void QMemoryLayoutRender::BuildInterface()
{
    this->RedrawDoubleBuffer();    
}

void QMemoryLayoutRender::resizeEvent(QResizeEvent* event)
{
    this->RedrawDoubleBuffer();
}

void QMemoryLayoutRender::paintEvent(QPaintEvent* paintEvent)
{
    qDebug() << "QMemoryLayoutRender widget size during paint:" << this->size();
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->m_doubleBuffer);
}

void QMemoryLayoutRender::RedrawDoubleBuffer()
{
    QPixmap newBuffer(size());
    QPainter painter(&newBuffer);

    auto pixelsPer5mm = getPixelsInMetric(this, 5);
    auto lastXPos = size().width() - 1;
    auto lastYPos = size().height() - 1;

    newBuffer.fill(colorLevelM1);

    painter.setPen(QPen(QColor(colorGrid), 1));
    for (qreal xHover = 0; xHover <= lastXPos; xHover += pixelsPer5mm)
    {        
        painter.drawLine(QPointF(xHover, (qreal)0), QPointF(xHover, lastYPos));
    }

    for (qreal yHover = 0; yHover <= lastYPos; yHover += pixelsPer5mm)
    {
        painter.drawLine(QPointF(0, (qreal)yHover), QPointF(lastXPos, yHover));        
    }

    painter.setPen(QPen(QColor(colorBorder), 1));
    painter.drawLine(0, 0, lastXPos, 0);
    painter.drawLine(0, 0, 0, lastYPos);
    painter.drawLine(lastXPos, lastYPos, lastXPos, 0);
    painter.drawLine(lastXPos, lastYPos, 0, lastYPos);

    this->m_doubleBuffer = newBuffer;
}

void QMemoryLayoutRender::SetModel(const SharedPtrVector<CMemorySection>& model)
{
    this->m_model = model;
}

void QMemoryLayoutRender::wheelEvent(QWheelEvent* event)
{    
    emit this->evMouseWheel(event->angleDelta().x(), event->angleDelta().y());
    event->accept();
}