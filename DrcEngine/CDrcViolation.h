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
        explicit CDrcViolation(std::vector<CRawEntry>&& involvedEntries, EDrcViolationCode violationCode)
            : CViolationBase(std::move(involvedEntries)), 
              m_violationCode(violationCode)
        {}

        /// @brief Simplified constructor accepting only CRawEntry. the @see {involvedEntries} only
        explicit CDrcViolation(CRawEntry involvedEntry, EDrcViolationCode violationCode)
            : CViolationBase(std::vector<CRawEntry> { involvedEntry }),
              m_violationCode(violationCode)
        {}
        
        /// @brief Default constructor
        explicit CDrcViolation(
                   std::vector<CRawEntry>&& involvedEntries, 
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EDrcViolationCode violationCode)
            : CViolationBase(std::move(involvedEntries), entryBeforeViolation, entryAfterViolation),
              m_violationCode(violationCode)
        {}

        /// @brief Compact constructor
        explicit CDrcViolation(
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EDrcViolationCode violationCode)
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
