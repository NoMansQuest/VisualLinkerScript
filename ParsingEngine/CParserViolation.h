#ifndef CPARSERVIOLATION_H__
#define CPARSERVIOLATION_H__

#include <numeric>
#include <vector>
#include "EParserViolationCode.h"
#include "../Models/CViolationBase.h"
#include "Models/ESeverityCode.h"

namespace VisualLinkerScript::ParsingEngine 
{
    using namespace VisualLinkerScript::Models;

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
        explicit CParserViolation(
				std::vector<CRawEntry>&& involvedEntries, 
				EParserViolationCode violationCode, 
				ESeverityCode severity = ESeverityCode::Error)
            : CViolationBase(severity),
    		  m_violationCode(violationCode),
              m_involvedEntries(std::move(involvedEntries))
        {}

        /// @brief Simplified constructor accepting only CRawEntry. the @see {involvedEntries} only
        explicit CParserViolation(
				CRawEntry involvedEntry,
				EParserViolationCode violationCode,
				ESeverityCode severity = ESeverityCode::Error)
            : CViolationBase(severity),
    	      m_violationCode(violationCode),
              m_involvedEntries(std::vector<CRawEntry> { involvedEntry })
        {}

        /// @brief Constructor with @see {involvedEntries}, @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CParserViolation(
                   std::vector<CRawEntry>&& involvedEntries,
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EParserViolationCode violationCode,
				   ESeverityCode severity = ESeverityCode::Error)
            : CViolationBase(severity),
    	      m_violationCode(violationCode),
              m_entryBeforeViolation(entryBeforeViolation),
              m_entryAfterViolation(entryAfterViolation),
              m_involvedEntries(std::move(involvedEntries))
        {}

        /// @brief Constructor with only @see {entryBeforeViolation} and @see {entryAfterViolation}
        explicit CParserViolation(
                   CRawEntry entryBeforeViolation,
                   CRawEntry entryAfterViolation,
                   EParserViolationCode violationCode,
				   ESeverityCode severity = ESeverityCode::Error)
            : CViolationBase(severity),
    		  m_violationCode(violationCode),
              m_entryBeforeViolation(entryBeforeViolation),
              m_entryAfterViolation(entryAfterViolation)
        {}

    public:
        /// @brief Returns back a list of raw entries involved in the current parsed component of
        ///        the current linker script.
        const std::vector<CRawEntry>& InvoledEntries() { return this->m_involvedEntries; }

        /// @brief Reports back the raw entry just before the violation starts
        CRawEntry EntryBeforeViolation() { return this->m_entryBeforeViolation; }

        /// @brief Reports back the raw entry just after the violation ends
        CRawEntry EntryAfterViolation() { return this->m_entryAfterViolation; }

        /// @brief Reports back the tpe of violation        
        EParserViolationCode Code() { return this->m_violationCode; }

        /// @brief Type of violation.
        virtual EViolationType Type() override { return EViolationType::ParserViolation; }

        /// @brief Produces the offending content 
		std::string GetOffendingContent(const std::shared_ptr<CRawFile>& rawFile)
        {
	        if (!this->InvoledEntries().empty())
	        {
                return std::accumulate(this->InvoledEntries().begin(), this->InvoledEntries().end(), std::string(),
                    [&rawFile](const std::string& acc, const CRawEntry& rawEntry) {
                        return acc + rawFile->ResolveRawEntry(rawEntry);
                    });
	        }
            else if (this->EntryBeforeViolation().EntryType() != RawEntryType::NotPresent)
            {
                return rawFile->ResolveRawEntry(this->EntryBeforeViolation());
            }
            else if (this->EntryAfterViolation().EntryType() != RawEntryType::NotPresent)
            {
                return rawFile->ResolveRawEntry(this->EntryAfterViolation());
            }
            else
            {
                return "";
            }
        }
    };
}

#endif // CPARSERVIOLATION_H__
