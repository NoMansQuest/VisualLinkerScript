#ifndef CSECTION_OUTPUT_H__
#define CSECTION_OUTPUT_H__

#include "CAddressedRegion.h"
#include "Helpers.h"
#include "SMetricRectangleF.h"
#include "SMetricSizeF.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	class CGraphicContext;

	/// @brief Represents an 'section output' object found in a given section statement.
	class CSectionOutput : public CAddressedRegion
	{		
		DECLARE_READONLY_PROPERTY(std::string, Content)
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, ContentArea)

		/// @brief Default constructor.
		explicit CSectionOutput(
				std::string content, 
				const uint32_t inModelStartPosition, 
				const uint32_t inModelLength, 
				const std::string& startAddress,
				const std::string& endAddress,
				const std::string& memorySize) :
			CAddressedRegion(
				inModelStartPosition,
				inModelLength,
				startAddress,
				endAddress,
				memorySize),
			m_Content(std::move(content))
		{}

		/// @copydoc CDrawableObjectBase::Paint
		void Paint(
			const CGraphicContext& graphicContext,
			QPainter& painter) override;

		/// @copydoc CAddressedRegion::CalculateBodySize
		SMetricSizeF CalculateBodySize(const CGraphicContext& graphicContext) const override;

		/// @copydoc CAddressedRegion::SetBodyPosition
		void SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext) override;

	protected:
		/// @brief Not used for this object
		int SizeMarkerDepth() override { return 0; }
	};
}

#endif // CSECTION_OUTPUT_H__
