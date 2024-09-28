#ifndef CPHDRS_STATEMENT_H__
#define CPHDRS_STATEMENT_H__

#include <vector>
#include <memory>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single entry in the PHDRS region
    class CPhdrsStatement : public CLinkerScriptContentBase
    {   
    private:
        CRawEntry m_headerNameEntry;
        CRawEntry m_headerTypeEntry;
        CRawEntry m_fileHdrEntry;
        std::shared_ptr<CLinkerScriptContentBase> m_atAddressFunction;
        std::shared_ptr<CLinkerScriptContentBase> m_flagsFunction;
        CRawEntry m_semicolonEntry;

    public:
        /// @brief Default constructor, accessible to inheritors only        
        explicit CPhdrsStatement(const CRawEntry& headerNameEntry,
                                 const CRawEntry& headerTypeEntry,
                                 const CRawEntry& fileHdrEntry,
                                 const std::shared_ptr<CLinkerScriptContentBase>& atAddressFunction,
                                 const std::shared_ptr<CLinkerScriptContentBase>& flagsFunction,
                                 const CRawEntry& semicolonEntry,
                                 const std::vector<CRawEntry>& rawElements, 
                                 const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(rawElements, violations),
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
            return ContentType::PhdrsStatement;
        }

        /// @brief Reports back the Header-Name Entry
        const CRawEntry& HeaderNameEntry() const
        {
            return this->m_headerNameEntry;
        }

        /// @brief Reports back the Header-Type Entry
        const CRawEntry& HeaderTypeEntry() const
        {
            return this->m_headerTypeEntry;
        }

        /// @brief Reports back the 'AT(ADDRESS)'
        const std::shared_ptr<CLinkerScriptContentBase> AtAddressFunction() const
        {
            return this->m_atAddressFunction;
        }

        /// @brief Reports back the Flags function
        const std::shared_ptr<CLinkerScriptContentBase> FlagsFunction() const
        {
            return this->m_flagsFunction;
        }

        /// @brief Reports back the Semicolon Entry
        const CRawEntry& SemicolonEntry() const
        {
            return this->m_semicolonEntry;
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
