#ifndef CPHDRS_STATEMENT_H__
#define CPHDRS_STATEMENT_H__

#include <vector>
#include "CExpression.h"
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a single entry in the PHDRS region
    class CPhdrsStatement : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CPhdrsStatement(CRawEntry headerName,
                                 CRawEntry headerType,
                                 CRawEntry fileHdr,
                                 std::vector<CRawEntry>&& atAddress,
                                 CExpression         
                                 std::vector<CRawEntry>&& rawElements, 
                                 std::vector<CViolation>&& violations) 
            : CLinkerScriptContentBase(rawElements, violations)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::PhdrsStatement;
        }
    };
}

#endif
