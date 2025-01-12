#include "QMemoryLayoutRender.h"
#include <QPainter>
#include "ColorResources.h"
#include "Composition/CFloorPlan.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer;

constexpr double logicalAreaTopMm = -1500;
constexpr double logicalAreaBottomMm = 1500;
constexpr double logicalAreaLeftMm = -1500;
constexpr double logicalAreaRight = 1500;
constexpr double gridUnitSizeMm = 5;

void QMemoryLayoutRender::BuildInterface()
{
    this->RedrawDoubleBuffer();
    this->repaint();
}

void QMemoryLayoutRender::resizeEvent(QResizeEvent* event)
{
    this->RedrawDoubleBuffer();
    this->repaint();
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

    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::VerticalSubpixelPositioning, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::LosslessImageRendering, true);

    auto graphicContext = CGraphicContext::Make(this);

    // Get scroll positions in pixels
    auto scrollX_InPixels = (qreal)Graphical::GetPixelsInMetric(graphicContext.DpiX(), this->m_scrollXmm) * -1;
    auto scrollY_InPixels = (qreal)Graphical::GetPixelsInMetric(graphicContext.DpiY(), this->m_scrollYmm) * -1;

    // Draw background color
    newBuffer.fill(Colors::ViewBackgroundColor);

    // Set scale and translation
    painter.scale(this->m_zoom, this->m_zoom);   
    painter.translate(scrollX_InPixels + (this->width() / 2), scrollY_InPixels + (this->height() / 2));

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
    auto pixelsPerMmX = Graphical::GetPixelsInMetric(dpiX, 1);
    auto pixelsPerMmY = Graphical::GetPixelsInMetric(dpiY, 1);

    auto yStartInPixels = logicalAreaTopMm * pixelsPerMmY;
    auto yEndInPixels = logicalAreaBottomMm * pixelsPerMmY;

    auto xStartInPixels = logicalAreaTopMm * pixelsPerMmY;
    auto xEndInPixels = logicalAreaBottomMm * pixelsPerMmY;

    painter.setPen(QPen(QColor::fromRgba(VisualLinkerScript::Components::MemoryVisualizer::Colors::ViewBackgroundGridColor), 1));
    for (qreal xHover = logicalAreaLeftMm; xHover <= logicalAreaRight; xHover += gridUnitSizeMm)
    {
        auto xInPixels = xHover * pixelsPerMmX;        
        painter.drawLine(QPointF(xInPixels, yStartInPixels), QPointF(xInPixels, yEndInPixels));
    }

    for (qreal yHover = logicalAreaTopMm; yHover <= logicalAreaBottomMm; yHover += gridUnitSizeMm)
    {
        auto yInPixels = yHover * pixelsPerMmY;
        painter.drawLine(QPointF(xStartInPixels, (qreal)yInPixels), QPointF(xEndInPixels, yInPixels));
    }

    // Draw grid center
    painter.setPen(QPen(QColor::fromRgba(VisualLinkerScript::Components::MemoryVisualizer::Colors::GridCenterGridColor), 2));
    painter.drawLine(QPointF(0, yStartInPixels), QPointF(0, yEndInPixels));
    painter.drawLine(QPointF(xStartInPixels, 0), QPointF(xEndInPixels, 0));

    /*
    painter.setPen(QPen( QColor::fromRgba(Colors::ViewBackgroundBorderColor), 1));
    painter.drawLine(0, 0, lastXPos, 0);
    painter.drawLine(0, 0, 0, lastYPos);
    painter.drawLine(lastXPos, lastYPos, lastXPos, 0);
    painter.drawLine(lastXPos, lastYPos, 0, lastYPos);
    */
}

void QMemoryLayoutRender::SetModel(const std::shared_ptr<CFloorPlan>& model)
{
    this->m_model = model;
    this->RedrawDoubleBuffer();
    this->repaint();
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
        this->RedrawDoubleBuffer();
        this->repaint();
    }
}

void QMemoryLayoutRender::SetZoom(const double zoom)
{
	if (this->m_zoom != zoom)
	{
        this->m_zoom = zoom;
        this->RedrawDoubleBuffer();
        this->repaint();
	}
}