#ifndef CPHDRS_STATEMENT_H__
#define CPHDRS_STATEMENT_H__

#include <vector>
#include <memory>
#include "Models/CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single entry in the PHDRS region
    class CPhdrsStatement : public CParsedContentBase
    {   
    private:
        CRawEntry m_headerNameEntry;
        CRawEntry m_headerTypeEntry;
        CRawEntry m_fileHdrEntry;
        std::shared_ptr<CParsedContentBase> m_atAddressFunction;
        std::shared_ptr<CParsedContentBase> m_flagsFunction;
        CRawEntry m_semicolonEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only        
        explicit CPhdrsStatement(const CRawEntry& headerNameEntry,
                                 const CRawEntry& headerTypeEntry,
                                 const CRawEntry& fileHdrEntry,
                                 const std::shared_ptr<CParsedContentBase>& atAddressFunction,
                                 const std::shared_ptr<CParsedContentBase>& flagsFunction,
                                 const CRawEntry& semicolonEntry,
                                 const std::vector<CRawEntry>& rawElements, 
                                 const LinqVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_headerNameEntry(headerNameEntry),
              m_headerTypeEntry(headerTypeEntry),
              m_fileHdrEntry(fileHdrEntry),
              m_atAddressFunction(atAddressFunction),
              m_flagsFunction(flagsFunction),
              m_semicolonEntry(semicolonEntry)
        {}        

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::ProgramHeaderStatement;
        }

        /// @brief Reports back the Header-Name Entry
        [[nodiscard]] const CRawEntry& HeaderNameEntry() const
        {
            return this->m_headerNameEntry;
        }

        /// @brief Reports back the Header-Type Entry
        [[nodiscard]] const CRawEntry& HeaderTypeEntry() const
        {
            return this->m_headerTypeEntry;
        }

        /// @brief Reports back the 'AT(ADDRESS)'
        [[nodiscard]] std::shared_ptr<CParsedContentBase> AtAddressFunction() const
        {
            return this->m_atAddressFunction;
        }

        /// @brief Reports back the Flags function
        [[nodiscard]] std::shared_ptr<CParsedContentBase> FlagsFunction() const
        {
            return this->m_flagsFunction;
        }

        /// @brief Reports back the Semicolon Entry
        [[nodiscard]] const CRawEntry& SemicolonEntry() const
        {
            return this->m_semicolonEntry;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] virtual const LinqVector<CViolationBase> AggregateViolation() const;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
