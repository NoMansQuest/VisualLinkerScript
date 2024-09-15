#ifndef PARSER_VIOLATION_ENTRY_H_
#define PARSER_VIOLATION_ENTRY_H_

#include <utility>
#include "CViolationEntryBase.h"

namespace VisualLinkerScript::ParsingEngine
{
    class CParserViolation;
}

/// @brief Entity represents a single parser violation.
class CParserViolationEntry : public CViolationEntryBase
{

public:
    /// @brief Creates an instance of CParserViolationEntry.
    CParserViolationEntry(
        std::shared_ptr<VisualLinkerScript::ParsingEngine::CParserViolation> parserViolation,
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
        m_parserViolation(std::move(parserViolation))
    {}

    /// @breif Default destructor.
    ~CParserViolationEntry() = default;

private:
    std::shared_ptr<VisualLinkerScript::ParsingEngine::CParserViolation> m_parserViolation;
};

#endif