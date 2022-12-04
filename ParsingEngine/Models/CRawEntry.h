#ifndef CRAWENTRY_H__
#define CRAWENTRY_H__

#include "EntryTypes.h"

namespace VisualLinkerScript { namespace ParsingEngine { namespace Models {

using namespace VisualLinkerScript::ParsingEngine::Models;

    /// @brief Represents an entry found in the LinkerScript file by the @see CPreliminaryParser
    class CRawEntry
    {
    public:
        /// @brief Explicitly made based on callers request
        explicit CRawEntry(EntryTypesEnum typeOfEntry,
                  unsigned int startPosition,
                  unsigned int length,
                  unsigned int depth)
            : m_entryType(typeOfEntry),
              m_startPosition(startPosition),
              m_length(length),
              m_depth(depth)
        {            
            // No operation to undertake
        }

        /// @brief Object-Copy constructor
        CRawEntry(CRawEntry& cloneSource)
            : m_entryType(cloneSource.getEntryType()),
              m_startPosition(cloneSource.getStartPosition()),
              m_length(cloneSource.getLength()),
              m_depth(cloneSource.getDepth())
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
        unsigned int m_depth;
        EntryTypesEnum m_entryType;
        
    public:
        /// @brief Reports back the type of thie entry
        /// @return Type of this entry
        EntryTypesEnum EntryType() const
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

        /// @brief Reports the 'Depth' of this piece of data. This referes to how deep this portion
        ///        is relative to curly-brackets defined regions.
        unsigned int Depth() const
        {
            return this->m_depth;
        }
    };

}}}

#endif
