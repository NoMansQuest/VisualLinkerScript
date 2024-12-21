#ifndef CMEMORY_REGION_H__
#define CMEMORY_REGION_H__

#include <vector>
#include "CParsedContentBase.h"
#include "Raw/CRawEntry.h"

using namespace VisualLinkerScript::Models::Raw;

namespace VisualLinkerScript::Models
{
    /// @brief Represents the 'MEMORIES' region in the linker-script
    class CMemoryRegion : public CParsedContentBase
    {       
    private:
        SharedPtrVector<CParsedContentBase> m_memoryStatements;
        CRawEntry m_headerTag;
        CRawEntry m_openingBracket;
        CRawEntry m_closingBracket;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CMemoryRegion(const CRawEntry& headerTag,
                               const CRawEntry& openingBracket,
                               const CRawEntry& closingBracket,
                               const SharedPtrVector<CParsedContentBase>& memoryStatements,
                               const std::vector<CRawEntry>& rawElements,
                               const SharedPtrVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_memoryStatements(memoryStatements),
              m_headerTag(headerTag),
              m_openingBracket(openingBracket),
              m_closingBracket(closingBracket)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::MemoryRegion;
        }

        /// @brief Reports back the statements
        const SharedPtrVector<CParsedContentBase>& Statements() const
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

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const SharedPtrVector<CViolationBase> AggregateViolation() const override;

        /// @brief Produces debug information on what this object represents.
        const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
