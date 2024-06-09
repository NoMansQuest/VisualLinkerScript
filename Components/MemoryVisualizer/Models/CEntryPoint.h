#ifndef CENTRYPOINT_H__
#define CENTRYPOINT_H__

#include "../../../Helpers.h"

#include <string>
#include "CContentBase.h"

/// @brief Represents the "ENTRY()" directive.
class CEntryPoint : public CContentBase
{
    DECLARE_STANDARD_PROPERTY(std::string, EntryPointContent)

public:
    /// @brief Default Constructor;
    CEntryPoint(bool isExternal) :
        CContentBase(true, isExternal)
    {}

    /// @brief Destructor
    ~CEntryPoint()
    {}
};

#endif // CMMEORYOBJECT_H__
