#ifndef CADDRESSED_REGION_H__
#define CADDRESSED_REGION_H__

#include <vector>
#include <string>
#include <cstdint>

#include "CDrawableObjectBase.h"
#include "CInteractiveObject.h"
#include "CModelMappedObject.h"
#include "SLineF.h"
#include "SMetricRectangleF.h"
#include "SMetricSizeF.h"

class QFontMetrics;

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
    /// @brief Represents an object that holds a start, end and size.
    class CAddressedRegion :
		public CModelMappedObject,
		public CInteractiveObject,
		public CDrawableObjectBase
    {
        DECLARE_READONLY_PROPERTY(std::string, StartAddress)
        DECLARE_READONLY_PROPERTY(std::string, EndAddress)
        DECLARE_READONLY_PROPERTY(std::string, MemorySize)        

        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, AddressStartTextArea)
        DECLARE_STANDARD_PROPERTY(std::string, AddressStartText)
        DECLARE_STANDARD_PROPERTY(SLineF, AddressStartConnectingLine)
        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, AddressEndTextArea)
        DECLARE_STANDARD_PROPERTY(std::string, AddressEndText)
        DECLARE_STANDARD_PROPERTY(SLineF, AddressEndConnctingLine)        

        DECLARE_STANDARD_PROPERTY(SLineF, SizeMarkerUpperConnector)
        DECLARE_STANDARD_PROPERTY(SLineF, SizeMarkerLowerConnector)
        DECLARE_STANDARD_PROPERTY(SLineF, SizeMarkerCenterConnector)
        DECLARE_STANDARD_PROPERTY(SLineF, SizeMarkerVerticalLine)
        DECLARE_STANDARD_PROPERTY(SLineF, SizeMarkerLabelLine)
        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, SizeMarkerTextArea)
        DECLARE_STANDARD_PROPERTY(std::string, SizeMarkerText)

        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, BodyArea)

        /// @brief Default constructor
        CAddressedRegion(
				const uint32_t inLinkerScriptStartPosition,
				const uint32_t inLinkerScriptLength,
				std::string startAddress, 
				std::string endAddress, 
				std::string memorySize) :
			CModelMappedObject(inLinkerScriptStartPosition, inLinkerScriptLength),
			m_StartAddress(std::move(startAddress)),
			m_EndAddress(std::move(endAddress)),
			m_MemorySize(std::move(memorySize))
        {}

        /// @brief Calculate the minimum amount of space this region would need.
        virtual SMetricSizeF CalculateBodySize(const CGraphicContext& graphicContext) const = 0;

        /// @brief Reports back whether the starting address is known
        bool StartAddressKnown() const { return !this->m_StartAddress.empty(); }

        /// @brief Reports back whether the ending address is known
    	bool EndAddressKnown() const { return !this->m_EndAddress.empty(); }

        /// @brief Reports back whether the size of the region is known
        bool MemorySizeKnown() const { return !this->m_MemorySize.empty(); }

        /// @brief Updates the coordinates of all involved objects based on the given allocated area.
        virtual void SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext) = 0;

    protected:

        /// @brief The higher the depth, the further the size-marker will be from
        ///        the body on the right side.
        virtual int SizeMarkerDepth() = 0;

        /// @brief Draws body-area, size-marker and address-marker
        void PaintAddressedRegion(
            const CGraphicContext& graphicContext,
            QPainter& painter,
            const QPen& bodyBorderPen,
            const QBrush& bodyFillBrush) const;
    };
}

#endif // CADDRESSED_REGION_H__
