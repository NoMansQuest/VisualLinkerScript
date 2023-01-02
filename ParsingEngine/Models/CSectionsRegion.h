#ifndef CSECTIONS_REGION_H__
#define CSECTIONS_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents the 'MEMORIES' region in the linker-script
    class CSectionsRegion : public CLinkerScriptContentBase
    {       
    private:
        std::vector<CLinkerScriptContentBase> m_entries;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CSectionsRegion(std::vector<CRawEntry>&& rawElements, 
                                 std::vector<CLinkerScriptContentBase>&& entries, 
                                 std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations),
              m_entries(std::move(entries))
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SectionsRegion;
        }

        /// @brief Reports back the statements
        const std::vector<CLinkerScriptContentBase>& Entries()
        {
            return m_entries;
        }
    };
}

#endif
