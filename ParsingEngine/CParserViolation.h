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
    class CParserViolation : CViolationBase
    {
    private:
        EParserViolationCode m_violationCode;

    public:
        /// @brief Default constructor, reporting the @see {involvedEntries} only
        explicit CParserViolation(std::vector<CRawEntry>&& involvedEntries, EParserViolationCode violationCode) 
            : CViolationBase(std::move(involvedEntries)), 
              m_violationCode(violationCode)
        {}

        /// @brief Simplified constructor accepting only CRawEntry. the @see {involvedEntries} only
        explicit CParserViolation(CRawEntry involvedEntry, EParserViolationCode violationCode)
            : CViolationBase(std::move(std::vector<CRawEntry> { involvedEntry })),
              m_violationCode(violationCode)
        {}

        
        /// @brief Constructor with @see {involvedEntries}, @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CParserViolation(
                   std::vector<CRawEntry>&& involvedEntries, 
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EParserViolationCode violationCode) 
            : CViolationBase(involvedEntries, entryBeforeViolation, entryAfterViolation),
              m_violationCode(violationCode)
        {}

        /// @brief Constructor with only @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CParserViolation(
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EParserViolationCode violationCode) 
            : CViolationBase(entryBeforeViolation, entryAfterViolation),
              m_violationCode(violationCode)
        {}        

    public:
        /// @brief Reports back the tpe of violation        
        const EParserViolationCode Code()
        {
            return m_violationCode;
        }
    };
}

#endif // CPARSERVIOLATION_H__
