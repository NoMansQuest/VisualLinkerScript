#ifndef CVERSION_SCOPE_H__
#define CVERSION_SCOPE_H__

#include <vector>
#include "CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a single entry in a version-node inside the "VERSIONS" region
    class CVersionScope : public CLinkerScriptContentBase
    {   
    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param versionName Name of the version we'de defining
        /// @param versionDependsOn (Optional) Name of the version this entry extends upon.
        /// @param openingBracket Entry containing the opening curly-brakcet
        /// @param closingBracket Entry containing the closing brakcet
        /// @param semicolonDelimiter Entry containing the semicolon delimiter
        /// @param composingRawElements A list of object this element is comprised of.
        explicit CVersionScope(CRawEntry verisonName,
                               CRawEntry versionDependsOn,
                               CRawEntry openingBracket,
                               CRawEntry closingBracket,
                               CRawEntry semicolonDelimiter,
                               std::vector<CRawEntry>&& rawElements,
                               std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations))
        {}        

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::VersionScope;
        }
    };
}

#endif
