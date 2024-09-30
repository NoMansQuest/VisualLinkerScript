#ifndef CCOMMENT_H__
#define CCOMMENT_H__

#include <vector>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a comment entry
    class CComment : public CParsedContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param composingRawElements A list of object this element is comprised of.
        /// @param violations Violations list.
        explicit CComment(const std::vector<CRawEntry>& composingRawElements,                              
                          const SharedPtrVector<CViolationBase>& violations)
            : CParsedContentBase(composingRawElements, violations)
        {}

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const SharedPtrVector<CViolationBase> AggregateViolation() const override
        {
            return this->Violations();
        }

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::Comment;
        }
    };
}

#endif
