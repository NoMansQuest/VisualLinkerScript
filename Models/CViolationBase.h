#ifndef CVIOLATIONBASE_H__
#define CVIOLATIONBASE_H__

#include <string>
#include <vector>
#include <memory>
#include "Raw/CRawEntry.h"

using namespace VisualLinkerScript::Models::Raw;

namespace VisualLinkerScript::Models 
{
    /// @brief This object contains base object for violations detected in the linkerscript
    class CViolationBase
    {
    private:
        CRawEntry m_entryBeforeViolation;
        CRawEntry m_entryAfterViolation;
        std::vector<CRawEntry> m_involvedEntries;  

    public:
        /// @brief Default constructor, reporting the @see {involvedEntries} only
        explicit CViolationBase(std::vector<CRawEntry>&& involvedEntries, EParserViolationCode violationCode) 
            : m_involvedEntries(std::move(involvedEntries))
        {}

        /// @brief Simplified constructor accepting only CRawEntry. the @see {involvedEntries} only
        explicit CViolationBase(CRawEntry involvedEntry, EParserViolationCode violationCode)
            : m_involvedEntries(std::move(std::vector<CRawEntry> { involvedEntry }))
        {}

        
        /// @brief Constructor with @see {involvedEntries}, @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CViolationBase(
                   std::vector<CRawEntry>&& involvedEntries, 
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EParserViolationCode violationCode) 
            : m_entryBeforeViolation(entryBeforeViolation),
              m_entryAfterViolation(entryAfterViolation),
              m_involvedEntries(std::move(involvedEntries))
        {}

        /// @brief Constructor with only @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CViolationBase(
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EParserViolationCode violationCode) 
            : m_entryBeforeViolation(entryBeforeViolation),
              m_entryAfterViolation(entryAfterViolation)
        {}        

    public:
        /// @brief Returns back a list of raw entries involved in the current parsed component of 
        ///        the current linker script.
        const std::vector<CRawEntry>& InvoledEntries()
        {
            return this->m_involvedEntries;
        }

        /// @brief Reports back the raw entry just before the violation starts        
        const CRawEntry EntryBeforeViolation()
        {
            return this->m_entryBeforeViolation;
        }

        /// @brief Reports back the raw entry just after the violation ends        
        const CRawEntry EntryAfterViolation()
        {
            return this->m_entryAfterViolation;
        }
    };
}

#endif // CVIOLATIONBASE_H__
