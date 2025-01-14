#ifndef CRAWENTRY_H__
#define CRAWENTRY_H__

#include "RawEntryType.h"
#include <cstdint>
#include <stdexcept>

namespace VisualLinkerScript::Models::Raw 
{
    /// @brief Represents an entry found in the LinkerScript file by the @see {CLinkerScriptLexer}
    class CRawEntry
    {
        bool m_diagnosticFlag;
        std::string m_diagnosticResolution;
        uint32_t m_startAbsPosition;
        uint32_t m_length;
        uint32_t m_parenthesisDepth;
        uint32_t m_scopeDepth;
        uint32_t m_startLineNumber;
        uint32_t m_endLineNumber;
        uint32_t m_startIndexInLine;
        uint32_t m_endIndexInLine;
        uint32_t m_parentVectorIndex;
        RawEntryType m_entryType;

    public:
        /// @brief Explicitly made based on callers request
        explicit CRawEntry(
	            const std::string& diagnosticResolution,
	            const RawEntryType typeOfEntry)
    	  : m_startAbsPosition(0),
            m_length(0),
            m_parenthesisDepth(0),
            m_scopeDepth(0),
            m_startLineNumber(0),
            m_endLineNumber(0),
            m_startIndexInLine(0),
            m_endIndexInLine(0),
            m_parentVectorIndex(0),
            m_entryType(typeOfEntry),
			m_diagnosticFlag(true),
			m_diagnosticResolution(diagnosticResolution)
        {  
        }


        /// @brief Explicitly made based on callers request
        explicit CRawEntry(
					const RawEntryType typeOfEntry,      
                    const uint32_t startLineNumber,
					const uint32_t startIndexInLine,
                    const uint32_t endLineNumber,
				    const uint32_t endIndexInLine,
                    const uint32_t startAbsPosition,
                    const uint32_t length,
                    const uint32_t parenthesisDepth,
                    const uint32_t scopeDepth)
            : m_startAbsPosition(startAbsPosition),
              m_length(length),
              m_parenthesisDepth(parenthesisDepth),
              m_scopeDepth(scopeDepth),
              m_startLineNumber(startLineNumber),
              m_endLineNumber(endLineNumber),
              m_startIndexInLine(startIndexInLine),
		      m_endIndexInLine(endIndexInLine),
              m_entryType(typeOfEntry),
			  m_diagnosticFlag(false)
        {}

        /// @brief Explicitly made based on callers request for single-line entries
        explicit CRawEntry(
					const RawEntryType typeOfEntry,
					const uint32_t parentVectorIndex,
                    const uint32_t lineNumber,
					const uint32_t startIndexInLine,
                    const uint32_t startAbsPosition,
                    const uint32_t length,
                    const uint32_t parenthesisDepth,
                    const uint32_t scopeDepth)
            : m_startAbsPosition(startAbsPosition),
              m_length(length),
              m_parenthesisDepth(parenthesisDepth),
              m_scopeDepth(scopeDepth),
              m_startLineNumber(lineNumber),
              m_endLineNumber(lineNumber),
              m_startIndexInLine(startIndexInLine),
              m_endIndexInLine(startIndexInLine + length - 1),
              m_entryType(typeOfEntry),
			  m_diagnosticFlag(false)
        {}

        /// @brief Explicitly constructs a "Null" entry, indicating that the entry does not exist.
        explicit CRawEntry()
            : m_startAbsPosition(0),
              m_length(0),
              m_parenthesisDepth(0),
              m_scopeDepth(0),
              m_startLineNumber(0),
              m_endLineNumber(0),
              m_startIndexInLine(0),
              m_endIndexInLine(0),
              m_parentVectorIndex(0),
              m_entryType(RawEntryType::NotPresent),
              m_diagnosticFlag(false)
        {}

        /// @brief Object-Copy constructor
        CRawEntry(const CRawEntry& cloneSource)
	        : m_startAbsPosition(cloneSource.StartPosition()),
	          m_length(cloneSource.Length()),
	          m_parenthesisDepth(cloneSource.ParenthesisDepth()),
	          m_scopeDepth(cloneSource.ScopeDepth()),
	          m_startLineNumber(cloneSource.StartLineNumber()),
	          m_endLineNumber(cloneSource.EndLineNumber()),
	          m_startIndexInLine(cloneSource.StartIndexInLine()),
	          m_endIndexInLine(cloneSource.EndIndexInLine()),
	          m_parentVectorIndex(cloneSource.ParentVectorIndex()),
	          m_entryType(cloneSource.EntryType()),
              m_diagnosticFlag(cloneSource.m_diagnosticFlag),
			  m_diagnosticResolution(cloneSource.m_diagnosticResolution)
        {}

        /// @brief Default destructor
        ~CRawEntry()
        {}

        /// @brief Returns content meant for diagnostics purposes.
        const std::string& DiagnosticContent() const
        {
            if (!this->m_diagnosticFlag)
            {
                throw std::invalid_argument("This CRawEntry was not marked as diagnostic entry (m_diagnosticFlag = false)");
            }
            return this->m_diagnosticResolution;
        }

        /// @brief Assignment operator
        CRawEntry& operator= (const CRawEntry& cloneSource)
        {
            this->m_startAbsPosition = cloneSource.StartPosition();
            this->m_parentVectorIndex = cloneSource.ParentVectorIndex();
            this->m_length = cloneSource.Length();
            this->m_parenthesisDepth = cloneSource.ParenthesisDepth();
            this->m_scopeDepth = cloneSource.ScopeDepth();
            this->m_startLineNumber = cloneSource.StartLineNumber();
            this->m_endLineNumber = cloneSource.EndLineNumber();
            this->m_startIndexInLine = cloneSource.StartIndexInLine();
            this->m_endIndexInLine = cloneSource.EndIndexInLine();
            this->m_entryType = cloneSource.EntryType();

            // Copy diagnostics data
            this->m_diagnosticFlag = cloneSource.m_diagnosticFlag;
            this->m_diagnosticResolution = cloneSource.m_diagnosticResolution;
            return *this;
        }
        
    public:
        /// @brief Reports back the type of thie entry
        /// @return Type of this entry
        RawEntryType EntryType() const
        { 
            return this->m_entryType; 
        }

        /// @brief Reports back whether this entry is present or serving as a placeholder
        bool IsPresent() const
        {
            return (this->m_entryType != RawEntryType::NotPresent);
        }

        /// @brief Returns the index of this entry in its parent vector.
        /// @remarks This is meant to speed up and simplify search and scan.
        uint32_t ParentVectorIndex() const
        {
            return this->m_parentVectorIndex;
        }

        /// @brief Reports back the length of this entry
        /// @return Length of this entry (>0)
        uint32_t Length() const
        {
            return this->m_length; 
        }

        /// @brief Reports back the starting position of this entry in the original stream
        /// @return Start position of this entry
        uint32_t StartPosition() const
        { 
            return this->m_startAbsPosition; 
        }

        /// @brief Line number where this entry begins on
        /// @return The line number
        uint32_t StartLineNumber() const
        {
            return this->m_startLineNumber;
        }

        /// @brief Starting index (in line) of the entry.
		/// @return The starting index in line.
        uint32_t StartIndexInLine() const
        {
            return this->m_startIndexInLine;
        }

        /// @brief Line number where this entry ends on
        /// @return The ending line number
        uint32_t EndLineNumber() const
        {
            return this->m_endLineNumber;
        }

        /// @brief Ending index (in line) of the entry.
		/// @return The ending index in line.
        uint32_t EndIndexInLine() const
        {
            return this->m_endIndexInLine;
        }

        /// @brief Reports the 'Parenthesis Depth' of this piece of data. This referes to how deep this portion
        ///        is relative to parenthesis-enclosued statement.
        uint32_t ParenthesisDepth() const
        {
            return this->m_parenthesisDepth;
        }

        /// @brief Reports the 'Depth' of this piece of data. This referes to how deep this portion
        ///        is relative to curly-brackets defined regions.
        uint32_t ScopeDepth() const
        {
            return this->m_scopeDepth;
        }
    };
}

#endif
