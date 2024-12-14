#include "QMemoryLayoutRender.h"
#include <QPainter>
#include "ColorResources.h"
#include "Composition/CFloorPlan.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer;

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

    auto graphicContext = CGraphicContext::Make(this);

    // Get scroll positions in pixels
    auto scrollX_InPixels = Graphical::GetPixelsInMetric(graphicContext.DpiX(), this->m_scrollXmm) * -1;
    auto scrollY_InPixels = Graphical::GetPixelsInMetric(graphicContext.DpiY(), this->m_scrollYmm) * -1;

    // Draw background color
    newBuffer.fill(Colors::ViewBackgroundColor);

    // Set scale and translation
    painter.scale(this->m_zoom, this->m_zoom);   
    painter.translate(
        scrollX_InPixels + this->width() / 2, 
        scrollY_InPixels + this->height() / 2);

    // Draw grids
    this->DrawGrids(painter, graphicContext.DpiX(), graphicContext.DpiY());

    // Draw all components
    if (this->m_model != nullptr)
    {
		this->m_model->Paint(graphicContext, painter);	    
    }

    // Set new buffer to current buffer.
    this->m_doubleBuffer = newBuffer;
}

void QMemoryLayoutRender::DrawGrids(QPainter& painter, double dpiX, double dpiY) const
{
    auto pixelsPer5mm = Graphical::GetPixelsInMetric(dpiX, 5);
    auto lastXPos = size().width() - 1;
    auto lastYPos = size().height() - 1;

    painter.setPen(QPen(QColor::fromRgb(VisualLinkerScript::Components::MemoryVisualizer::Colors::ViewBackgroundGridColor), 1));
    for (qreal xHover = 0; xHover <= lastXPos; xHover += pixelsPer5mm)
    {
        painter.drawLine(QPointF(xHover, (qreal)0), QPointF(xHover, lastYPos));
    }

    for (qreal yHover = 0; yHover <= lastYPos; yHover += pixelsPer5mm)
    {
        painter.drawLine(QPointF(0, (qreal)yHover), QPointF(lastXPos, yHover));
    }

    painter.setPen(QPen( QColor::fromRgba(Colors::ViewBackgroundBorderColor), 1));
    painter.drawLine(0, 0, lastXPos, 0);
    painter.drawLine(0, 0, 0, lastYPos);
    painter.drawLine(lastXPos, lastYPos, lastXPos, 0);
    painter.drawLine(lastXPos, lastYPos, 0, lastYPos);
}

void QMemoryLayoutRender::SetModel(const std::shared_ptr<CFloorPlan>& model)
{
    this->m_model = model;
    this->RedrawDoubleBuffer();
}

void QMemoryLayoutRender::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        // Note: angle-delta is in 8th of a degree. One step is 15 degrees        
        auto degreeDelta = (int)(event->angleDelta().y() / 8);
        emit this->evZoomChanged(degreeDelta); // +/- 15% per step (depending on direction)
    }
    else
    {
        if (event->buttons() & Qt::MiddleButton)
        {
            emit this->evMouseWheel(event->angleDelta().y(), 0);
        }
        else
        {
            emit this->evMouseWheel(0, event->angleDelta().y());
        }
    }    

    event->accept();
}

void QMemoryLayoutRender::SetScrollPosition(const int scrollXmm, const int scrollYmm)
{
    bool redrawNeeded = (scrollXmm != this->m_scrollXmm) || (scrollYmm != this->m_scrollYmm);
    this->m_scrollXmm = scrollXmm;
    this->m_scrollYmm = scrollYmm;
    if (redrawNeeded)
    {
        this->update();
    }
}

void QMemoryLayoutRender::SetZoom(const double zoom)
{
	if (this->m_zoom != zoom)
	{
        this->m_zoom = zoom;
        this->update();
	}
}