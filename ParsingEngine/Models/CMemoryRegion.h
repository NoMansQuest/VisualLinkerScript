#ifndef CMEMORY_REGION_H__
#define CMEMORY_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Raw/CRawEntry.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents the 'MEMORIES' region in the linker-script
    class CMemoryRegion : public CLinkerScriptContentBase
    {       
    private:
        std::vector<CMemoryStatement> m_memoryStatements;
        CRawEntry m_headerTag;
        CRawEntry m_openingBracket;
        CRawEntry m_closingBracket;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CMemoryRegion(CRawEntry headerTag,
                               CRawEntry openiningBracket,
                               CRawEntry closingBracket, 
                               std::vector<CRawEntry>&& rawElements, 
                               std::vector<CMemoryStatement>&& memoryStatements, 
                               std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations),
              m_memoryStatements(std::move(memoryStatements))
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::MemoryRegion;
        }

        /// @brief Reports back the statements
        std::vector<CMemoryStatement>& Statements() const
        {
            return m_memoryStatements;
        }

        /// @brief Gets the header tag in raw-element
        CRawEntry& HeaderTag() const
        {
            return this->m_headerTag;
        }

        /// @brief Gets the opening-bracket's raw-element
        CRawEntry& OpeningBracket() const
        {
            return this->m_openingBracket;
        }

        /// @brief Gets the closing-bracket's raw-element
        CRawEntry& ClosingBracket() const
        {
            return this->m_closingBracket;
        }
    };
}

#endif
