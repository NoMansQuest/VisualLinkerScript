#ifndef CUNRECOGNIZABLE_CONTENT_H__
#define CUNRECOGNIZABLE_CONTENT_H__

#include <vector>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single 'Output' segment in the "SECTIONS"
    class CUnrecognizableContent : public CParsedContentBase
    {   
    public:
        /// @brief Construct an "Unrecognizable" object
        explicit CUnrecognizableContent(const std::vector<CRawEntry>& composingRawElements, 
                                        const SharedPtrVector<CViolationBase>& violations = {})
            : CParsedContentBase(composingRawElements, violations)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::UnrecognizableContent;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const SharedPtrVector<CViolationBase> AggregateViolation() const override {
            return this->Violations();
        }
    };
}


#endif
