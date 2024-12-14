#ifndef CSECTION_DEFINITION_BASE_H__
#define CSECTION_DEFINITION_BASE_H__

#include <vector>
#include <string>
#include <cstdint>

#include "CAddressedRegion.h"
#include "CFillExpressionButton.h"
#include "CProgramHeaderButton.h"
#include "../../../Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
    /// @brief Represents a Memory-Statement.
    class CSectionDefinitionBase : public CAddressedRegion
    {
        DECLARE_READONLY_PROPERTY(std::string, Title)
        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, TitleArea)
        DECLARE_STANDARD_PROPERTY(SMetricRectangleF, HeaderArea)
	    DECLARE_STANDARD_PROPERTY(CFillExpressionButton, FillExpression)
        DECLARE_STANDARD_PROPERTY(SharedPtrVector<CProgramHeaderButton>, ProgramHeaders)

        /// @brief Default constructor
        CSectionDefinitionBase(
	            std::string title,
	            CFillExpressionButton fillExpression, 
	            const SharedPtrVector<CProgramHeaderButton>& programHeaders,
				const uint32_t inModelStartPosition,
				const uint32_t inModelLength,
	            const std::string& startAddress,
	            const std::string& endAddress,
	            const std::string& memorySize) :
			 CAddressedRegion(inModelStartPosition, inModelLength, startAddress, endAddress, memorySize),
		     m_Title(std::move(title)),
		     m_FillExpression(std::move(fillExpression)),
		     m_ProgramHeaders(programHeaders)
	    {}
    };
}

#endif // CMEMORYSTATEMENT_H