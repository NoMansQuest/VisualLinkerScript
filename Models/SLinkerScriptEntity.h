#ifndef __LINKER_SCRIPT__
#define __LINKER_SCRIPT__

#include <memory>
#include <vector>
#include "SModelBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a Linker-Script
    struct SLinkerScriptEntity : SModelBase
    {   
        /// @brief Content of this linker-script
        std::vector<std::shared_ptr<SModelBase>> Content;

        /// @brief Type of the object, must be defined by each type.
        virtual EModelType Type() { return EModelType::Root; }
    };
}

#endif
