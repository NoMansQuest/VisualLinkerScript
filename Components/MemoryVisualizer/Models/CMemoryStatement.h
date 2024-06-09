#ifndef CMEMORYSTATEMENT_H
#define CMEMORYSTATEMENT_H

#include <vector>
#include <string>
#include <cstdint>
#include "../../../Helpers.h"
#include "CSizableObject.h"

/// @brief Represents a Memory-Statement.
class CMemoryStatement : public CSizableObject
{
    DECLARE_STANDARD_PROPERTY( std::string, Title )
    DECLARE_STANDARD_PROPERTY( std::string, FillExpression )
    DECLARE_STANDARD_PROPERTY( std::vector<std::string>, PhdrList )
    DECLARE_READONLY_PROPERTY( SharedPtrVector<CContentBase>, ChildContent )

public:
    /// @brief Default constructor
    CMemoryStatement(bool isExternal)
        : CSizableObject(true, isExternal)
    {}
};

#endif // CMEMORYSTATEMENT_H
