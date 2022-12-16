#ifndef CMEMORY_REGION_H__
#define CMEMORY_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents the 'MEMORIES' region in the linker-script
    class CMemoryRegion : public CLinkerScriptContentBase
    {       
    private:
        std::vector<CMemoryStatement> m_rawElements;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CMemoryRegion(std::vector<CRawEntry>&& rawElements, 
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
        const std::vector<CMemoryStatement>& Statements()
        {
            return m_rawElements;
        }
    }
}

#endif