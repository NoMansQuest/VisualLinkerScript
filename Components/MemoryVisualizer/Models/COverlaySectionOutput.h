#ifndef COVERLAY_SECTION_OUTPUT_H__
#define COVERLAY_SECTION_OUTPUT_H__

#include "Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
	/// @brief Represents an 'Overlay' section found in "OVERLAY" statements.
	class COverlaySectionOutput : public CAddressedRegion
	{
		DECLARE_READONLY_PROPERTY(std::string, Content)

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

		/// @copydoc CAddressedRegion::CalculateDesiredSize
		SMetricSizeF CalculateDesiredSize(const QFontMetrics& fontMetrics) override;

		/// @copydoc CAddressedRegion::SetGeometry
		void SetGeometry(SMetricRectangleF allocatedArea) override;
	};
}

#endif // COVERLAY_SECTION_OUTPUT_H__
