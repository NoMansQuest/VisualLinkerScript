#include "QMemoryVisualizer.h"
#include <qboxlayout.h>
#include <qguiapplication.h>

#include "QPercentageLineEdit.h"
#include "QMemoryLayoutRender.h"
#include "Composition/CFloorPlan.h"
#include "Composition/CMemoryRegionBlock.h"
#include "Composition/COverlayStatement.h"
#include "Composition/CSectionStatement.h"
#include "LinkerScriptManager/QLinkerScriptManager.h"

#define FORCE_RANGE(value, upper, lower) ((value > upper) ? upper : (value < lower) ? lower : value)

void QMemoryVisualizer::BuildInterface()
{
    this->m_masterLayout = new QVBoxLayout();
    this->m_rendererAndVScrollHolderLayout = new QHBoxLayout();
    this->m_hScrollAndButtonsHolderLayout = new QHBoxLayout();
    this->m_hScrollHousingLayout = new QVBoxLayout();
    this->m_memoryLayoutRender = new QMemoryLayoutRender();

    this->m_verticalScrollBar = new QScrollBar(Qt::Vertical);
    this->m_verticalScrollBar->setFixedWidth(13);
    this->m_verticalScrollBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->m_verticalScrollBar->setMaximum(1500);
    this->m_verticalScrollBar->setMinimum(-1500);
    this->m_verticalScrollBar->setValue(0);
    this->m_verticalScrollBar->setPageStep(100);
    this->m_verticalScrollBar->setSingleStep(10);

    this->m_horizontalScrollBar = new QScrollBar(Qt::Horizontal);
    this->m_horizontalScrollBar->setFixedHeight(13);    
    this->m_horizontalScrollBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->m_horizontalScrollBar->setMaximum(1500);
    this->m_horizontalScrollBar->setMinimum(-1500);
    this->m_horizontalScrollBar->setValue(0);
    this->m_horizontalScrollBar->setPageStep(100);
    this->m_horizontalScrollBar->setSingleStep(10);

    this->m_zoomInButton = new QPushButton();
    this->m_zoomInButton->setFixedWidth(20);
    this->m_zoomInButton->setFixedHeight(20);
    this->m_zoomInButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->m_zoomInButton->setIcon(QIcon(":/resources/Images/zoom_in_icon.svg").pixmap(QSize(20, 20)));
    this->m_zoomInButton->setCursor(Qt::PointingHandCursor);
    this->m_zoomInButton->setToolTip("Zoom in (+)");

    this->m_zoomOutButton = new QPushButton();
    this->m_zoomOutButton->setFixedWidth(20);
    this->m_zoomOutButton->setFixedHeight(20);
    this->m_zoomOutButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->m_zoomOutButton->setIcon(QIcon(":/resources/Images/zoom_out_icon.svg").pixmap(QSize(20, 20)));
    this->m_zoomOutButton->setCursor(Qt::PointingHandCursor);
    this->m_zoomOutButton->setToolTip("Zoom out (-)");

    this->m_zoomToContent = new QPushButton();
    this->m_zoomToContent->setFixedWidth(20);
    this->m_zoomToContent->setFixedHeight(20);
    this->m_zoomToContent->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->m_zoomToContent->setIcon(QIcon(":/resources/Images/zoom_to_content_icon.svg").pixmap(QSize(20, 20)));
    this->m_zoomToContent->setCursor(Qt::PointingHandCursor);
    this->m_zoomToContent->setToolTip("Zoom to content");

    this->m_currentZoomTextEdit = new QPercentageLineEdit(nullptr, 200);
    this->m_currentZoomTextEdit->setAlignment(Qt::AlignCenter);
    this->m_currentZoomTextEdit->setFixedHeight(20);
    this->m_currentZoomTextEdit->setFixedWidth(50);
    this->m_currentZoomTextEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->m_memoryLayoutRender->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	this->m_rendererAndVScrollHolderLayout->addWidget(this->m_memoryLayoutRender, 1);
    this->m_rendererAndVScrollHolderLayout->addWidget(this->m_verticalScrollBar, 0);
    this->m_rendererAndVScrollHolderLayout->setSpacing(2);
    this->m_rendererAndVScrollHolderLayout->setContentsMargins(0, 0, 0, 0);    

    this->m_hScrollHousingLayout->addSpacing(7);
    this->m_hScrollHousingLayout->addWidget(this->m_horizontalScrollBar, 1);
    this->m_hScrollHousingLayout->setContentsMargins(0, 0, 0, 0);
    this->m_hScrollHousingLayout->setSpacing(0);

    this->m_hScrollAndButtonsHolderLayout->addLayout(this->m_hScrollHousingLayout, 1);
    this->m_hScrollAndButtonsHolderLayout->addWidget(this->m_zoomOutButton, 0);
    this->m_hScrollAndButtonsHolderLayout->addWidget(this->m_currentZoomTextEdit, 0);
    this->m_hScrollAndButtonsHolderLayout->addWidget(this->m_zoomInButton, 0);
    this->m_hScrollAndButtonsHolderLayout->addWidget(this->m_zoomToContent, 0);
    this->m_hScrollAndButtonsHolderLayout->setSpacing(2);        
    
    this->m_masterLayout->setSpacing(0);
    this->m_masterLayout->addLayout(this->m_rendererAndVScrollHolderLayout, 1);
    this->m_masterLayout->addSpacing(5);
    this->m_masterLayout->addLayout(this->m_hScrollAndButtonsHolderLayout, 0);
    this->m_masterLayout->setContentsMargins(0, 0, 0, 0);

    QObject::connect(this->m_memoryLayoutRender, &QMemoryLayoutRender::evMouseWheel, this, &QMemoryVisualizer::OnScrollChangeByMouseWheel);
    QObject::connect(this->m_memoryLayoutRender, &QMemoryLayoutRender::evZoomChanged, this, &QMemoryVisualizer::OnZoomChangedByMouseWheel);
    QObject::connect(this->m_zoomInButton, &QPushButton::clicked, this, &QMemoryVisualizer::OnZoomIncreaseClicked);
    QObject::connect(this->m_zoomOutButton, &QPushButton::clicked, this, &QMemoryVisualizer::OnZoomDecreaseClicked);
    QObject::connect(this->m_horizontalScrollBar, &QScrollBar::valueChanged, this, &QMemoryVisualizer::OnScrollPositionChange);
    QObject::connect(this->m_verticalScrollBar, &QScrollBar::valueChanged, this, &QMemoryVisualizer::OnScrollPositionChange);
    QObject::connect(this->m_currentZoomTextEdit, &QLineEdit::textChanged, this, &QMemoryVisualizer::OnZoomUpdated);
    this->setLayout(this->m_masterLayout);        
}

void QMemoryVisualizer::CalculateAndUpdateModelGeometry() const
{
    if (!this->screen())
    {
        throw std::exception("'Screen' turned out null");
    }

    auto graphicContext = CGraphicContext::Make(this);
    auto bodySize = this->m_model->CalculateBodySize(graphicContext);
    auto newLeft =  -(bodySize.CX() / 2);
    auto newTop = -(bodySize.CY() / 2);
    SMetricRectangleF repositionedModel(newLeft, newTop, bodySize.CX(), bodySize.CY());
    this->m_model->SetBodyPosition(repositionedModel, graphicContext);
}

void QMemoryVisualizer::SetModel(const std::shared_ptr<CFloorPlan>& floorPlan)
{
    this->m_model = floorPlan;
    this->CalculateAndUpdateModelGeometry();    
    this->m_memoryLayoutRender->SetModel(floorPlan);
    this->RequestRedraw();    
}

void QMemoryVisualizer::RequestRedraw() const
{
    this->m_memoryLayoutRender->repaint();    
}

void QMemoryVisualizer::OnScrollChangeByMouseWheel(const int xSteps, const int ySteps) const
{
    
    auto newHScrollValue = FORCE_RANGE(this->m_horizontalScrollBar->value() - (xSteps / 2), this->m_horizontalScrollBar->maximum(), this->m_horizontalScrollBar->minimum());
    auto newVScrollValue = FORCE_RANGE(this->m_verticalScrollBar->value() - (ySteps / 2), this->m_verticalScrollBar->maximum(), this->m_verticalScrollBar->minimum());
    this->m_horizontalScrollBar->setValue(newHScrollValue);
    this->m_verticalScrollBar->setValue(newVScrollValue);
}

void QMemoryVisualizer::OnZoomChangedByMouseWheel(int zoomChangeInPercent) const
{
    auto previousZoom = this->m_currentZoomTextEdit->Percentage();
    auto newZoom = std::min(200, std::max(1, this->m_currentZoomTextEdit->Percentage() + zoomChangeInPercent));
    if (newZoom != previousZoom)
    {
        this->m_currentZoomTextEdit->SetPercentage(newZoom);
    }
}

void QMemoryVisualizer::OnScrollPositionChange() const
{
    this->m_memoryLayoutRender->SetScrollPosition(
        this->m_horizontalScrollBar->value(), 
        this->m_verticalScrollBar->value());    
}

void QMemoryVisualizer::OnZoomIncreaseClicked() const
{
    auto previousZoom = this->m_currentZoomTextEdit->Percentage();
    auto newZoom = std::min(this->m_currentZoomTextEdit->Percentage() + 5, 200);
    if (newZoom != previousZoom)
    {
        this->m_currentZoomTextEdit->SetPercentage(newZoom);
    }
}

void QMemoryVisualizer::OnZoomDecreaseClicked() const
{
    auto previousZoom = this->m_currentZoomTextEdit->Percentage();
    auto newZoom = std::max(this->m_currentZoomTextEdit->Percentage() - 5, 1);
    if (newZoom != previousZoom)
    {
        this->m_currentZoomTextEdit->SetPercentage(newZoom);
    }
}

void QMemoryVisualizer::OnZoomUpdated() const
{
    this->m_memoryLayoutRender->SetZoom((double)this->m_currentZoomTextEdit->Percentage() / 100);
}
