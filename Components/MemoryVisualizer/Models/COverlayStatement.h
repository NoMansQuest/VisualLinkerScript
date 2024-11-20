#ifndef COVERLAY_STATEMENT_H__
#define COVERLAY_STATEMENT_H__

#include <vector>
#include <string>
#include <cstdint>

#include "CAddressedContent.h"
#include "../../../Helpers.h"
#include "COverlaySectionStatement.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents an 'Overlay' object found in "SECTION"
    class COverlayStatement : public CAddressedContent
    {       
        DECLARE_READONLY_PROPERTY(SharedPtrVector<COverlaySectionStatement>, Sections)

    protected:
        ~COverlayStatement() = default;

    public:
        /// @brief Default constructor.
        COverlayStatement() :            
			m_Sections(SharedPtrVector<COverlaySectionStatement>())
        {	        
        }
    };
}

#endif // COVERLAY_STATEMENT_H__
