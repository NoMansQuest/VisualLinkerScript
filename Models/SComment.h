#ifndef SCOMMENT_H
#define SCOMMENT_H

#include <string>
#include <cstdlib>
#include "SModelBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Base object for every element in the Model
    struct SComment : SModelBase
    {
        /// @copydoc
        virtual EModelType Type() override { return EModelType::Comment; }
    };
};

#endif // SCOMMENT_H
