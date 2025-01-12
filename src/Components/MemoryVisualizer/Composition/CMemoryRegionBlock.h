#ifndef CMEMORY_REGION_COMPOSITION_H__
#define CMEMORY_REGION_COMPOSITION_H__

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
	constexpr double sectionToSectionVSpaceMm = 2;
    constexpr double emptyMemoryRegionHeight = 15;

    /// @brief Various types of memory region block.
    enum class MemoryRegionBlockFlag
    {
	    Default,
        Orphan,
        Violation
    };

    /// @brief Represents a Memory-Object.
    class CMemoryRegionBlock : public CAddressedRegion
    {
        DECLARE_READONLY_PROPERTY(std::string, Name)
        DECLARE_READONLY_PROPERTY(std::string, Title)
        DECLARE_READONLY_PROPERTY(std::string, MemorySizeText)
        DECLARE_READONLY_PROPERTY(MemoryRegionBlockFlag, Flag)
	    DECLARE_READONLY_PROPERTY(std::shared_ptr<LinqVector<CSectionDefinitionBase>>, ChildContent)

        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, TitleArea)
        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, MemorySizeTextArea)

        /// @brief Default constructor
        CMemoryRegionBlock(
				const std::string& name,
				const std::string& title,
				const std::string& memorySizeText,
				const MemoryRegionBlockFlag flag,
				const std::shared_ptr<LinqVector<CSectionDefinitionBase>>& childContent,
		        const uint32_t inLinkerScriptStartPosition,
		        const uint32_t inLinkerScriptLength,
				const std::string& startAddress,
				const std::string& endAddress,
				const std::string& memorySize) :
			  CAddressedRegion(inLinkerScriptStartPosition, inLinkerScriptLength, startAddress, endAddress, memorySize),
			  m_Name(name),
	          m_Title(title),
    		  m_MemorySizeText(memorySizeText),
			  m_Flag(flag),
              m_ChildContent(childContent)
        {}

    public:
        /// @copydoc CDrawableObjectBase::Paint
        void Paint(const CGraphicContext& graphicContext, QPainter& painter) override;

        /// @copydoc CAddressedRegion::CalculateBodySize
		SMetricSizeF CalculateBodySize(const CGraphicContext& graphicContext) const override;
                
        /// @copydoc CAddressedRegion::SetBodyPosition
		void SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext) override;

    protected:
        /// @brief Memory-Region is three levels deep
        int SizeMarkerDepth() override { return 20; }
    };
}

#endif // CMEMORY_REGION_COMPOSITION_H__
