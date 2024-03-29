#ifndef CMEMORY_REGION_H__
#define CMEMORY_REGION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawEntry.h"

using namespace VisualLinkerScript::Models::Raw;

namespace VisualLinkerScript::Models
{
    /// @brief Represents the 'MEMORIES' region in the linker-script
    class CMemoryRegion : public CLinkerScriptContentBase
    {       
    private:
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_memoryStatements;
        CRawEntry m_headerTag;
        CRawEntry m_openingBracket;
        CRawEntry m_closingBracket;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CMemoryRegion(CRawEntry headerTag,
                               CRawEntry openiningBracket,
                               CRawEntry closingBracket,
                               std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& memoryStatements,
                               std::vector<CRawEntry>&& rawElements,
                               SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_memoryStatements(std::move(memoryStatements)),
              m_headerTag(headerTag),
              m_openingBracket(openiningBracket),
              m_closingBracket(closingBracket)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::MemoryRegion;
        }

        /// @brief Reports back the statements
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& Statements() const
        {
            return m_memoryStatements;
        }

        /// @brief Gets the header tag in raw-element
        const CRawEntry& HeaderTag() const
        {
            return this->m_headerTag;
        }

        /// @brief Gets the opening-bracket's raw-element
        const CRawEntry& OpeningBracket() const
        {
            return this->m_openingBracket;
        }

        /// @brief Gets the closing-bracket's raw-element
        const CRawEntry& ClosingBracket() const
        {
            return this->m_closingBracket;
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
