#include "QMemoryLayoutRender.h"
#include <qguiapplication.h>
#include <QPainter>


// Helper functions
namespace
{
	constexpr QColor colorLevelM1(0x10, 0x10, 0x10);
	constexpr QColor colorGrid(0x20, 0x20, 0x20);
    constexpr QColor colorBorder(0x30, 0x30, 0x30);


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

    double dpiX = 0;
	double dpiY = 0;
    if (this->screen())
    {
        dpiX = this->screen()->physicalDotsPerInchX();
        dpiY = this->screen()->physicalDotsPerInchY();
    }
    else
    {
        qWarning("No screen provided!");
        return;
    }

    // Draw background color
    newBuffer.fill(colorLevelM1);

    // Draw grids
    this->DrawGrids(painter, dpiX, dpiY);


    // Set new buffer to current buffer.
    this->m_doubleBuffer = newBuffer;
}

void QMemoryLayoutRender::DrawGrids(QPainter& painter, double dpiX, double dpiY) const
{
    auto pixelsPer5mm = Graphical::GetPixelsInMetric(dpiX, 5);
    auto lastXPos = size().width() - 1;
    auto lastYPos = size().height() - 1;


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
}

void QMemoryLayoutRender::SetModel(const SharedPtrVector<CMemoryRegion>& model)
{
    this->m_model = model;
}

void QMemoryLayoutRender::wheelEvent(QWheelEvent* event)
{    
    emit this->evMouseWheel(event->angleDelta().x(), event->angleDelta().y());
    event->accept();
}