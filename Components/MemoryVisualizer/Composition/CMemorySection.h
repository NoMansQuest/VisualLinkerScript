#ifndef CMEMORY_SECTION_H__
#define CMEMORY_SECTION_H__

#include <vector>
#include <string>
#include <cstdint>

#include "CSectionDefinitionBase.h"
#include "../../Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
    /// @brief Represents a Memory-Object.
    class CMemorySection : public CAddressedRegion
    {
        DECLARE_READONLY_PROPERTY(std::string, Title)
        DECLARE_READONLY_PROPERTY(std::string, MemorySizeText)
	    DECLARE_READONLY_PROPERTY(SharedPtrVector<CSectionDefinitionBase>, ChildContent)

        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, TitleArea)
        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, MemorySizeTextArea)

        /// @brief Default constructor
        CMemorySection(
				std::string title,
				std::string memorySizeText,
				const SharedPtrVector<CSectionDefinitionBase>& childContent,
		        const uint32_t inModelStartPosition,
		        const uint32_t inModelLength) :
			  CAddressedRegion(inModelStartPosition, inModelLength, true, true, true),
	          m_Title(std::move(title)),
    		  m_MemorySizeText(std::move(memorySizeText)),
              m_ChildContent(childContent)
        {}

        /// @copydoc CDrawableObjectBase::Paint
        void Paint(
            const CGraphicContext& graphicContext,
            QPainter& painter) override;

        /// @copydoc CAddressedRegion::CalculateBodySize
        SMetricSizeF CalculateBodySize(const CGraphicContext& graphicContext) const override;

        /// @copydoc CAddressedRegion::SetBodyPosition
        void SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext) override;
    };
}

#endif // CMEMORY_SECTION_H__
