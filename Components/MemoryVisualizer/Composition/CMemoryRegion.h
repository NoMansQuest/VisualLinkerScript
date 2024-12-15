#ifndef CMEMORY_REGION_H__
#define CMEMORY_REGION_H__

#include <vector>
#include <string>
#include <cstdint>

#include "CSectionDefinitionBase.h"
#include "../../Helpers.h"
#include "Components/MemoryVisualizer/ColorResources.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	constexpr double textToMemoryRegionVSpaceMm = 2;
	constexpr double titleToSizeTextHSpaceMm = 2;
	constexpr double memoryRegionBorderMm = 0.5;
	constexpr double sectionToSectionVSpaceMm = 4;

    /// @brief Represents a Memory-Object.
    class CMemoryRegion : public CAddressedRegion
    {
        DECLARE_READONLY_PROPERTY(std::string, Title)
        DECLARE_READONLY_PROPERTY(std::string, MemorySizeText)
	    DECLARE_READONLY_PROPERTY(SharedPtrVector<CSectionDefinitionBase>, ChildContent)

        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, TitleArea)
        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, MemorySizeTextArea)

        /// @brief Default constructor
        CMemoryRegion(
				std::string title,
				std::string memorySizeText,
				const SharedPtrVector<CSectionDefinitionBase>& childContent,
		        const uint32_t inLinkerScriptStartPosition,
		        const uint32_t inLinkerScriptLength,
				const std::string& startAddress,
				const std::string& endAddress,
				const std::string& memorySize) :
			  CAddressedRegion(inLinkerScriptStartPosition, inLinkerScriptLength, startAddress, endAddress, memorySize),
	          m_Title(std::move(title)),
    		  m_MemorySizeText(std::move(memorySizeText)),
              m_ChildContent(childContent)
        {}

    public:
        /// @copydoc CDrawableObjectBase::Paint
        void Paint(const CGraphicContext& graphicContext, QPainter& painter) override
        {
			// Draw header & size
			painter.setFont(graphicContext.FontLarge());
			painter.setPen(QPen(QColor::fromRgb(Colors::MemoryRegionDefaultForeColor)));

			painter.drawText(
				this->TitleArea().ConvertToQRect(graphicContext),
				Qt::AlignHCenter | Qt::AlignVCenter,
				QString::fromStdString(this->Title()));

			painter.setFont(graphicContext.FontLargeBold());
			painter.drawText(
				this->MemorySizeTextArea().ConvertToQRect(graphicContext),
				Qt::AlignHCenter | Qt::AlignVCenter,
				QString::fromStdString(this->MemorySizeText()));


			const auto borderPen = QPen(QColor::fromRgb(Colors::MemoryRegionDefaultBorderColor), 2, Qt::DotLine, Qt::FlatCap, Qt::BevelJoin);
			const auto fillBrush = QBrush(QColor::fromRgba(Colors::MemoryRegionDefaultBackgroundColor), Qt::SolidPattern);
			this->PaintAddressedRegion(graphicContext, painter, borderPen, fillBrush);

			// Draw all children
			for (const auto& childContent : this->ChildContent())
			{
				childContent->Paint(graphicContext, painter);
			}
        }

        /// @copydoc CAddressedRegion::CalculateBodySize
        SMetricSizeF CalculateBodySize(const CGraphicContext& graphicContext) const override
        {
			// Top memory label
			auto topMemoryLabelAndSize = Graphical::GetTextWidthInPixels(this->Title() + this->MemorySizeText(), graphicContext.FontMetricsLarge());

			double calculatedWidth = 0;
			double calculatedHeight = (graphicContext.FontMetricsLarge().height() * 1.5) + (memoryRegionBorderMm * 2); // Initial height

			if (!this->m_ChildContent.empty())
			{
				for (const auto& child : this->m_ChildContent)
				{
					auto childDesiredSize = child->CalculateBodySize(graphicContext);
					calculatedWidth = std::max(calculatedWidth, childDesiredSize.CX());
					calculatedHeight += childDesiredSize.CY() + sectionToSectionVSpaceMm;
				}
			}

			return { std::max(topMemoryLabelAndSize, calculatedWidth), calculatedHeight };
        }
        
        /// @copydoc CAddressedRegion::SetBodyPosition
        void SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext) override
        {
			auto calculatedDesiredSize = this->CalculateBodySize(graphicContext);
			auto currentYHolder = allocatedArea.Top();

			auto topMemoryLabelRect =
				SMetricRectangleF(graphicContext.FontMetricsLarge().boundingRect(QString::fromStdString(this->m_Title)), graphicContext.DpiX(), graphicContext.DpiY())
				.Offset(allocatedArea.Left(), allocatedArea.Top());

			auto topMemorySizeRect =
				SMetricRectangleF(graphicContext.FontMetricsLarge().boundingRect(QString::fromStdString(this->m_MemorySizeText)), graphicContext.DpiX(), graphicContext.DpiY())
				.Offset(allocatedArea.Left() + topMemoryLabelRect.Width() + titleToSizeTextHSpaceMm, allocatedArea.Top());

			this->SetTitleArea(topMemoryLabelRect);
			this->SetMemorySizeTextArea(topMemorySizeRect);

			currentYHolder += graphicContext.FontMetricsLarge().height() + textToMemoryRegionVSpaceMm;
			double memoryRegionTop = currentYHolder;

			if (!this->m_ChildContent.empty())
			{
				for (const auto& child : this->m_ChildContent)
				{
					auto childDesiredSize = child->CalculateBodySize(graphicContext);
					auto childAllocatedArea = SMetricRectangleF(allocatedArea.Left(), currentYHolder, childDesiredSize.CX(), childDesiredSize.CY());
					child->SetBodyPosition(childAllocatedArea, graphicContext);
					currentYHolder += childDesiredSize.CY() + sectionToSectionVSpaceMm;
				}
			}

			double memoryRegionBottom = currentYHolder;
			double memoryAreaHeight = memoryRegionBottom - memoryRegionTop;

			this->SetBodyArea(SMetricRectangleF(allocatedArea.Left(), memoryRegionTop, calculatedDesiredSize.CX(), memoryAreaHeight));

			// Set size-marker and address marker
            CAddressedRegion::SetBodyPosition(this->BodyArea(), graphicContext);
        }

    protected:
        /// @brief Memory-Region is three levels deep
        int SizeMarkerDepth() override { return 25; }
    };
}

#endif // CMEMORY_REGION_H__
