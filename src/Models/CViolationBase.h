#ifndef CVIOLATIONBASE_H__
#define CVIOLATIONBASE_H__

#include "Models/ESeverityCode.h"
#include "Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::Models 
{
	using namespace VisualLinkerScript::Models::Raw;

    /// @brief Type of violation.
    enum class EViolationType
    {
        LexerViolation,
        DrcViolation,
        ParserViolation
    };

    /// @brief This object contains base object for violations detected in the linker script.
    class CViolationBase
    {
		private:
            ESeverityCode m_severity;			

		public:
			/// @brief ViolationBase constructor.
            explicit CViolationBase(const ESeverityCode severityCode)
	            : m_severity(severityCode)
            {}

			/// @brief Severity of the violation.
            [[nodiscard]] ESeverityCode Severity() const { return this->m_severity; }

			/// @brief Type of the violation.
		    virtual EViolationType Type() = 0;			
    };
}

#endif // CVIOLATIONBASE_H__
