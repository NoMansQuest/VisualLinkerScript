#ifndef COVERLAY_STATEMENT_H__
#define COVERLAY_STATEMENT_H__

#include <vector>
#include <string>
#include <cstdint>
#include "../../../Helpers.h"
#include "CContentBase.h"
#include "COverlaySectionStatement.h"
#include "CSectionDefinitionBase.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents an 'Overlay' object found in "SECTION"
    class COverlayStatement : CSectionDefinitionBase
    {       
        DECLARE_READONLY_PROPERTY(SharedPtrVector<COverlaySectionStatement>, Sections)

    protected:
        ~COverlayStatement() = default;

    public:
        /// @brief Default constructor.
        COverlayStatement(bool isExternal) :
            CSectionDefinitionBase(isExternal),
			m_Sections(SharedPtrVector<COverlaySectionStatement>())
        {	        
        }
    };
}

#endif // COVERLAY_STATEMENT_H__
