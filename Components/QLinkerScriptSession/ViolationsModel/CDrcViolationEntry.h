#ifndef DRC_VIOLATION_ENTRY_H_
#define DRC_VIOLATION_ENTRY_H_

#include <utility>
#include "CViolationEntryBase.h"

namespace VisualLinkerScript::DrcEngine
{
	class CDrcViolation;	
}

/// @brief Entity represents a single DRC violation.
class CDrcViolationEntry : public CViolationEntryBase
{

public:
    /// @brief Creates an instance of CDrcViolationEntry.
    CDrcViolationEntry(
        std::shared_ptr<VisualLinkerScript::DrcEngine::CDrcViolation> drcViolation,
        QUuid uuid,
        std::string violationCode,
        std::string description,
        std::string affectedFile,
        const uint32_t lineNumber,
        const uint32_t columnNumber,
        std::shared_ptr<CViolationEntryBase> parent = nullptr)
        : CViolationEntryBase(
            uuid, 
            std::move(violationCode), 
            std::move(description),
            std::move(affectedFile), 
            lineNumber, 
            columnNumber,
            parent),
	      m_drcViolation(std::move(drcViolation))
    {}

    /// @breif Default destructor.
    ~CDrcViolationEntry() = default;

private:
    std::shared_ptr<VisualLinkerScript::DrcEngine::CDrcViolation> m_drcViolation;
};

#endif