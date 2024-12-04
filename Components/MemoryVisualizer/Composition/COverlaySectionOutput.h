#ifndef COVERLAY_SECTION_OUTPUT_H__
#define COVERLAY_SECTION_OUTPUT_H__

#include "CAddressedRegion.h"
#include "Helpers.h"
#include "SMetricRectangleF.h"
#include "SMetricSizeF.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	/// @brief Represents an 'Overlay' section found in "OVERLAY" statements.
	class COverlaySectionOutput : public CAddressedRegion
	{
		DECLARE_READONLY_PROPERTY(std::string, Content)
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, ContentArea)

		/// @brief Default constructor.
		COverlaySectionOutput(std::string content,
				const uint32_t inModelStartPosition,
				const uint32_t inModelLength,
				const bool startAddressKnown,
				const bool endAddressKnown,
				const bool memorySizeKnown) :
			CAddressedRegion(inModelStartPosition, inModelLength, startAddressKnown, endAddressKnown, memorySizeKnown),
			m_Content(std::move(content))
		{}

		/// @copydoc CDrawableObjectBase::Paint
		void Paint(const QPainter& painter) override;

		/// @copydoc CAddressedRegion::CalculateBodySize
		SMetricSizeF CalculateBodySize(
			const double dpiX,
			const double dpiY,
			const QFontMetrics& fontMetricsSmall,
			const QFontMetrics& fontMetricsLarge) override;

		/// @copydoc CAddressedRegion::SetBodyPosition
		void SetBodyPosition(
			SMetricRectangleF allocatedArea,
			const double dpiX,
			const double dpiY,
			const QFontMetrics& fontMetricsSmall,
			const QFontMetrics& fontMetricsLarge
		) override;
	};
}

#endif // COVERLAY_SECTION_OUTPUT_H__
