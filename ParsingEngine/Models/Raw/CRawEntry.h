#ifndef CRAWENTRY_H__
#define CRAWENTRY_H__

#include "RawEntryType.h"

namespace VisualLinkerScript::ParsingEngine::Models::Raw 
{
    /// @brief Represents an entry found in the LinkerScript file by the @see CPreliminaryParser
    class CRawEntry
    {
    public:
        /// @brief Explicitly made based on callers request
        explicit CRawEntry(RawEntryType typeOfEntry,                  
                    uint32_t startLineNumber,
                    uint32_t endLineNumber,
                    uint32_t startPosition,
                    uint32_t length,
                    uint32_t parenthesisDepth,
                    uint32_t scopeDepth)
            : m_startPosition(startPosition),
                m_length(length),
                m_parenthesisDepth(parenthesisDepth),
                m_scopeDepth(scopeDepth),
                m_startLineNumber(startLineNumber),
                m_endLineNumber(endLineNumber),
                m_entryType(typeOfEntry)
        {            
            // No operation to undertake
        }

        /// @brief Object-Copy constructor
        CRawEntry(const CRawEntry& cloneSource)
            : m_startPosition(cloneSource.StartPosition()),
                m_length(cloneSource.Length()),
                m_parenthesisDepth(cloneSource.ParenthesisDepth()),
                m_scopeDepth(cloneSource.ScopeDepth()),
                m_startLineNumber(cloneSource.StartLineNumber()),
                m_endLineNumber(cloneSource.EndLineNumber()),
                m_entryType(cloneSource.EntryType())
        {
            // No operation to undertake
        }

        ~CRawEntry()
        {
            // No operation to undertake
        }

    private:
        uint32_t m_startPosition;
        uint32_t m_length;
        uint32_t m_parenthesisDepth;
        uint32_t m_scopeDepth;
        uint32_t m_startLineNumber;
        uint32_t m_endLineNumber;
        RawEntryType m_entryType;
        
    public:
        /// @brief Reports back the type of thie entry
        /// @return Type of this entry
        RawEntryType EntryType() const
        { 
            return this->m_entryType; 
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
            return this->m_startPosition; 
        }

        /// @brief Line number where this entry begins on
        /// @return The line number
        uint32_t StartLineNumber() const
        {
            return this->m_startLineNumber;
        }

        /// @brief Line number where this entry ends on
        /// @return The ending line number
        uint32_t EndLineNumber() const
        {
            return this->m_endLineNumber;
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
