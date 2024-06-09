#ifndef CSTATEMENTOBJECT_H__
#define CSTATEMENTOBJECT_H__

#include <string>
#include "CContentBase.h"

/// @brief Type of statement object
enum class StatementObjectType : uint32_t{
    SymbolAssignment,
    PointerAssignment,
    FreeFunctionCall
};


/// @brief Represents statements, such as symbol-assignments and function-calls
class CStatementObject : CContentBase
{
    DECLARE_STANDARD_PROPERTY( SRectangleF, IconFrame )
    DECLARE_STANDARD_PROPERTY( SPointF, TextTopLeftPoint )
    DECLARE_STANDARD_PROPERTY( std::string, lValueContent )
    DECLARE_STANDARD_PROPERTY( std::string, rValueContent )
    DECLARE_STANDARD_PROPERTY( StatementObjectType, StatementType )

public:
    /// @brief Default constructor
    CStatementObject(bool isExternal) :
        CContentBase(true, isExternal)
    {}
};


#endif // CSTATEMENTOBJECT_H__
