#ifndef CSECTION_OUTPUT_H__
#define CSECTION_OUTPUT_H__

#include "CAddressedRegion.h"
#include "Helpers.h"
#include "SMetricRectangleF.h"
#include "SMetricSizeF.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
	/// @brief Represents an 'Overlap' object found in "SECTION"
	class CSectionOutput : public CAddressedRegion
	{		
		DECLARE_READONLY_PROPERTY(std::string, Content)

		/// @brief Default constructor.
		explicit CSectionOutput(
				std::string content, 
				const uint32_t inModelStartPosition, 
				const uint32_t inModelLength, 
				const bool startAddressKnown,
				const bool endAddressKnown,
				const bool memorySizeKnown) :
			CAddressedRegion(
				inModelStartPosition,
				inModelLength,
				startAddressKnown,
				endAddressKnown,
				memorySizeKnown),
			m_Content(std::move(content))
		{}

		/// @copydoc CAddressedRegion::CalculateDesiredSize
		SMetricSizeF CalculateDesiredSize(
			const QFontMetrics& fontMetricsSmall,
			const QFontMetrics& fontMetricsLarge) override;

		/// @copydoc CAddressedRegion::SetGeometry
		void SetGeometry(SMetricRectangleF allocatedArea) override;
	};
}

#endif // CSECTION_OUTPUT_H__
