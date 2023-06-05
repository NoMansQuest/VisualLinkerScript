#ifndef CDRCVIOLATION_H__
#define CDRCVIOLATION_H__

#include <string>
#include <vector>
#include <memory>
#include "EDrcViolationCode.h"
#include "../Models/CViolationBase.h"

using namespace VisualLinkerScript::Models;

namespace VisualLinkerScript::DrcEngine 
{
    /// @brief This object contains information about a detected violation in the Linker Script
    class CDrcViolation : CViolationBase
    {
    private:
        EDrcViolationCode m_violationCode;

    public:
        /// @brief Default constructor, reporting the @see {involvedEntries} only
        explicit CDrcViolation(std::vector<CRawEntry>&& involvedEntries, EParserViolationCode violationCode) 
            : CViolationBase(std::move(involvedEntries)), 
              m_violationCode(violationCode)
        {}

        /// @brief Simplified constructor accepting only CRawEntry. the @see {involvedEntries} only
        explicit CDrcViolation(CRawEntry involvedEntry, EParserViolationCode violationCode)
            : CViolationBase(std::move(std::vector<CRawEntry> { involvedEntry })),
              m_violationCode(violationCode)
        {}

        
        /// @brief Constructor with @see {involvedEntries}, @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CDrcViolation(
                   std::vector<CRawEntry>&& involvedEntries, 
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EParserViolationCode violationCode) 
            : CViolationBase(involvedEntries, entryBeforeViolation, entryAfterViolation),
              m_violationCode(violationCode)
        {}

        /// @brief Constructor with only @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CDrcViolation(
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EParserViolationCode violationCode) 
            : CViolationBase(entryBeforeViolation, entryAfterViolation),
              m_violationCode(violationCode)
        {}        

    public:
        /// @brief Reports back the tpe of violation        
        const EDrcViolationCode Code()
        {
            return m_violationCode;
        }
    };
}

#endif // CPARSERVIOLATION_H__
