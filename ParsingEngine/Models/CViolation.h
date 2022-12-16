#ifndef CVIOLATION_H__
#define CVIOLATION_H__

#include <string>
#include <vector>
#include "Raw/CRawEntry.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::Models 
{
    /// @brief An exhaustive list of all known violations.
    enum class ViolationCode : uint32_t
    {
        
    };

    /// @brief This object contains information about a detected violation in the Linker Script
    class CViolation
    {
    private:
        std::vector<CRawEntry> m_involvedEntries;        
        ViolationCode m_violationCode;

    public:
        /// @brief Default constructor.
        CViolation(std::vector<CRawEntry>&& involvedEntries, ViolationCode violationCode) 
            : m_involvedEntries(std::move(involvedEntries)), m_violationCode(violationCode)
        {
        }

    public:
        /// @brief Returns back a list of raw entries involved in the current parsed component of 
        ///        the current linker script 
        const std::vector<CRawEntry>& InvoledEntries()
        {
            return m_involvedEntries;
        }

        /// @brief Reports back the tpe of violation        
        const ViolationCode Code()
        {
            return m_violationType;
        }
    };
}

#endif
