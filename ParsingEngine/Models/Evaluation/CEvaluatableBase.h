#ifndef CEVALUATABLE_H__
#define CEVALUATABLE_H__

#include "../Raw/CRawEntry.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Abstract representation of an evaulatable entity.
    class CEvaluatableBase
    {
    public:
        /// @brief An entity could be positive or negative. The sign however may not be present especially if the
        ///        the entity is the first entry in a compound expression.  
        /// @return True if the sign of the entity is present (hence @see {SignRawEntry} being valid). False otherswise, 
        ///         which would translate into "positive" by default;
        virtual bool IsExplicitlySigned() = 0;
    };
    
    /// @brief Abstract representation of a signed evaulatable entity.
    class CSignedEvaluatableBase : public CEvaluatableBase
    {
    private:        
        CRawEntry m_signRawEntry;

    public: 
        /// @brief Default constructor
        CSignedEvaluatableBase(CRawEntry signRawEntry) :
            m_signRawEntry(signRawEntry)
        {}

        /// @brief Default destructor
        ~CSignedEvaluatableBase()
        {}

    public:
        /// @copydoc CEvaluatableBase::IsExplicitlySigned()
        virtual bool IsExplicitlySigned() override
        {
            return true;
        }

        /// @brief An entity could be positive or negative. The sign statement (if present) is provided here
        /// @return True if the sign of the entity is present (hence @see {SignRawEntry} being valid). False otherswise, 
        ///         which would translate into "positive" by default;
        const CRawEntry& SignRawEntry()
        {
            return this->m_signRawEntry;
        }
    };

    /// @brief Abstract representation of an unsigned evaulatable entity.
    class CUnsignedEvaluatableBase : CEvaluatableBase
    {
    public: 
        /// @brief Default constructor
        CUnsignedEvaluatableBase();
        ~CUnsignedEvaluatableBase();

    public:
        /// @copydoc CEvaluatableBase::IsExplicitlySigned()
        virtual bool IsExplicitlySigned() override
        {
            return false;
        }
    };
}



#endif
