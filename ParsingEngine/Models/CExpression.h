#ifndef CEXPRESSION_H__
#define CEXPRESSION_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a compound R-Value expression, composed of multiple symbols (and/or sub expressions).
    class CExpression: public CLinkerScriptContentBase
    {   
    private:
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_composition;

    public:
        /// @brief Parameterized constructor, accessible to inheritors only
        explicit CExpression(std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& composition,
                             std::vector<CRawEntry>&& rawElements, 
                             std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_composition(composition)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::Expression;
        }    

        /// @brief Gets the "Composition" 
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& Composition()
        {
            return this->m_composition;
        }
    };
}

#endif
