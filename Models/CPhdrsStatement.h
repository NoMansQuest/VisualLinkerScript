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
        /// @param rawElements A list of object this element is comprised of.
        explicit CPhdrsStatement(CRawEntry headerNameEntry,
                                 CRawEntry headerTypeEntry,
                                 CRawEntry fileHdrEntry,
                                 std::shared_ptr<CLinkerScriptContentBase> atAddressFunction,
                                 std::shared_ptr<CLinkerScriptContentBase> flagsFunction,
                                 CRawEntry semicolonEntry,
                                 std::vector<CRawEntry>&& rawElements, 
                                 std::vector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
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
        const CRawEntry& HeaderNameEntry()
        {
            return this->m_headerNameEntry;
        }

        /// @brief Reports back the Header-Type Entry
        const CRawEntry& HeaderTypeEntry()
        {
            return this->m_headerTypeEntry;
        }

        /// @brief Reports back the 'AT(ADDRESS)'
        const std::shared_ptr<CLinkerScriptContentBase> AtAddressFunction()
        {
            return this->m_atAddressFunction;
        }

        /// @brief Reports back the Flags function
        const std::shared_ptr<CLinkerScriptContentBase> FlagsFunction()
        {
            return this->m_flagsFunction;
        }

        /// @brief Reports back the Semicolon Entry
        const CRawEntry& SemicolonEntry()
        {
            return this->m_semicolonEntry;
        }
    };
}

#endif
