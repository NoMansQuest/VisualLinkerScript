#ifndef CRAWENTRY_H__
#define CRAWENTRY_H__

#include "EntryTypes.h"

using namespace VisualLinkerScript::ParsingEngine::Models;

namespace VisualLinkerScript { namespace ParsingEngine { namespace Models {

    /// @brief Represents an entry found in the LinkerScript file by the @see CPreliminaryParser
    class CRawEntry
    {
    public:
        explicit CRawEntry(EntryTypesEnum typeOfEntry, 
                  unsigned int startPosition,
                  unsigned int length)
            : m_entryType(typeOfEntry),
              m_startPosition(startPosition),
              m_length(length)
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
        EntryTypesEnum m_entryType;
        
    public:
        /// @brief Reports back the type of thie entry
        /// @return Type of this entry
        EntryTypesEnum getEntryType() const
        { 
            return this->m_entryType; 
        }

        /// @brief Reports back the length of this entry
        /// @return Length of this entry (>0)
        unsigned int getLength() const
        {
            return this->m_length; 
        }

        /// @brief Reports back the starting position of this entry in the original stream
        /// @return Start position of this entry
        unsigned int getStartPosition() const
        { 
            return this->m_startPosition; 
        }
    }
}}}

#endif