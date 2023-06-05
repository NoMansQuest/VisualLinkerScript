#ifndef CMEMORY_STATEMENT_H__
#define CMEMORY_STATEMENT_H__

#include <vector>
#include <memory>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single entry in the "MEMORIES" region
    class CMemoryStatement : public CLinkerScriptContentBase
    {   
    private:
        CRawEntry m_nameEntry;
        std::shared_ptr<CLinkerScriptContentBase> m_attributes;
        CRawEntry m_colonEntry;
        std::shared_ptr<CLinkerScriptContentBase> m_originAssignment;
        std::shared_ptr<CLinkerScriptContentBase> m_lengthAssignment;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CMemoryStatement(CRawEntry nameEntry,
                                  std::shared_ptr<CLinkerScriptContentBase> attributes,
                                  CRawEntry colonEntry,
                                  std::shared_ptr<CLinkerScriptContentBase> originAssignment,
                                  std::shared_ptr<CLinkerScriptContentBase> lengthAssignment,
                                  std::vector<CRawEntry>&& rawEntries,
                                  std::vector<CParserViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawEntries), std::move(violations)),
              m_nameEntry(nameEntry),
              m_attributes(attributes),
              m_colonEntry(colonEntry),
              m_originAssignment(originAssignment),
              m_lengthAssignment(lengthAssignment)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::MemoryStatement;
        }

        /// @brief Reports back 'Name' entry
        const CRawEntry NameEntry()
        {
            return m_nameEntry;
        }

        /// @brief Rpeorts back the Attributes of the statement (as CMemoryStatementAttribut
        const std::shared_ptr<CLinkerScriptContentBase> Attributes()
        {
            return m_attributes;
        }

        /// @brief Reports back 'Colon' entry
        const CRawEntry ColonEntry()
        {
            return m_colonEntry;
        }

        /// @brief Rpeorts back 'ORIGIN' or 'org' assignment
        const std::shared_ptr<CLinkerScriptContentBase> OriginAssignment()
        {
            return m_originAssignment;
        }

        /// @brief Reports back 'LENGTH' or 'l' or 'len' assignment
        std::shared_ptr<CLinkerScriptContentBase> LengthAssignment()
        {
            return m_lengthAssignment;
        }
    };    
}

#endif
