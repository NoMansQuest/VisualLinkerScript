#ifndef CPARSERVIOLATION_H__
#define CPARSERVIOLATION_H__

#include <string>
#include <vector>
#include <memory>
#include "EParserViolationCode.h"
#include "../Models/CViolationBase.h"

using namespace VisualLinkerScript::Models;

namespace VisualLinkerScript::ParsingEngine 
{
    /// @brief This object contains information about a detected violation in the Linker Script
    class CParserViolation : public CViolationBase
    {
    private:
        EParserViolationCode m_violationCode;

    private:
        CRawEntry m_entryBeforeViolation;
        CRawEntry m_entryAfterViolation;
        std::vector<CRawEntry> m_involvedEntries;

    public:
        /// @brief Default constructor, reporting the @see {involvedEntries} only
        explicit CParserViolation(std::vector<CRawEntry>&& involvedEntries, EParserViolationCode violationCode)
            : m_violationCode(violationCode),
              m_involvedEntries(std::move(involvedEntries))
        {}

        /// @brief Simplified constructor accepting only CRawEntry. the @see {involvedEntries} only
        explicit CParserViolation(CRawEntry involvedEntry, EParserViolationCode violationCode)
            : m_violationCode(violationCode),
              m_involvedEntries(std::vector<CRawEntry> { involvedEntry })
        {}

        /// @brief Constructor with @see {involvedEntries}, @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CParserViolation(
                   std::vector<CRawEntry>&& involvedEntries,
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EParserViolationCode violationCode)
            : m_violationCode(violationCode),
              m_entryBeforeViolation(entryBeforeViolation),
              m_entryAfterViolation(entryAfterViolation),
              m_involvedEntries(std::move(involvedEntries))
        {}

        /// @brief Constructor with only @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CParserViolation(
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EParserViolationCode violationCode)
            : m_violationCode(violationCode),
              m_entryBeforeViolation(entryBeforeViolation),
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

        /// @brief Reports back the tpe of violation        
        const EParserViolationCode Code()
        {
            return m_violationCode;
        }
    };
}

#endif // CPARSERVIOLATION_H__
