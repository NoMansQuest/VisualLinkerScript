#ifndef CSECTION_DEFINITION_BASE_H__
#define CSECTION_DEFINITION_BASE_H__

#include <vector>
#include <string>
#include <cstdint>

#include "CAddressedRegion.h"
#include "../../../Helpers.h"

class CProgramHeader;

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Memory-Statement.
    class CSectionDefinitionBase : public CAddressedRegion
    {
        DECLARE_READONLY_PROPERTY(std::string, Title)
        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, HeaderArea) // This is the green (or other colored) area visually on top of the section
        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, TitleArea)
	    DECLARE_STANDARD_PROPERTY(std::string, FillExpression)
        DECLARE_STANDARD_PROPERTY(std::vector<CProgramHeader>, ProgramHeaders)

        /// @brief Default constructor
        CSectionDefinitionBase(
	            std::string title, 
	            std::string fillExpression, 
	            const std::vector<CProgramHeader>& programHeaders,
				const uint32_t inModelStartPosition,
				const uint32_t inModelLength,
				const bool startAddressKnown,
				const bool endAddressKnown,
				const bool memorySizeKnown) :			
			 CAddressedRegion(inModelStartPosition, inModelLength, startAddressKnown, endAddressKnown, memorySizeKnown),
		     m_Title(std::move(title)),
		     m_FillExpression(std::move(fillExpression)),
		     m_ProgramHeaders(programHeaders)
	    {}

        /// @copydoc CAddressedRegion::CalculateDesiredSize
        SMetricSizeF CalculateDesiredSize(const QFontMetrics& fontMetrics) override;

        /// @copydoc CAddressedRegion::SetGeometry
        void SetGeometry(SMetricRectangleF allocatedArea) override;
    };
}

#endif // CMEMORYSTATEMENT_H