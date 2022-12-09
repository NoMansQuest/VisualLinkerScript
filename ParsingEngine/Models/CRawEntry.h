#ifndef CRAWENTRY_H__
#define CRAWENTRY_H__

#include "EntryType.h"

namespace VisualLinkerScript { namespace ParsingEngine { namespace Models {

using namespace VisualLinkerScript::ParsingEngine::Models;

    /// @brief Represents an entry found in the LinkerScript file by the @see CPreliminaryParser
    class CRawEntry
    {
    public:
        /// @brief Explicitly made based on callers request
        explicit CRawEntry(EntryType typeOfEntry,                  
                  unsigned int startLineNumber,
                  unsigned int endLineNumber,
                  unsigned int startPosition,
                  unsigned int length,
                  unsigned int parenthesisDepth,
                  unsigned int scopeDepth)
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
        unsigned int m_startPosition;
        unsigned int m_length;
        unsigned int m_parenthesisDepth;
        unsigned int m_scopeDepth;
        unsigned int m_startLineNumber;
        unsigned int m_endLineNumber;
        EntryType m_entryType;
        
    public:
        /// @brief Reports back the type of thie entry
        /// @return Type of this entry
        EntryType EntryType() const
        { 
            return this->m_entryType; 
        }

        /// @brief Reports back the length of this entry
        /// @return Length of this entry (>0)
        unsigned int Length() const
        {
            return this->m_length; 
        }

        /// @brief Reports back the starting position of this entry in the original stream
        /// @return Start position of this entry
        unsigned int StartPosition() const
        { 
            return this->m_startPosition; 
        }

        /// @brief Line number where this entry begins on
        /// @return The line number
        unsigned int StartLineNumber() const
        {
            return this->m_startLineNumber;
        }

        /// @brief Line number where this entry ends on
        /// @return The ending line number
        unsigned int EndLineNumber() const
        {
            return this->m_endLineNumber;
        }

        /// @brief Reports the 'Parenthesis Depth' of this piece of data. This referes to how deep this portion
        ///        is relative to parenthesis-enclosued statement.
        unsigned int ParenthesisDepth() const
        {
            return this->m_parenthesisDepth;
        }

        /// @brief Reports the 'Depth' of this piece of data. This referes to how deep this portion
        ///        is relative to curly-brackets defined regions.
        unsigned int ScopeDepth() const
        {
            return this->m_scopeDepth;
        }
    };

}}}

#endif
