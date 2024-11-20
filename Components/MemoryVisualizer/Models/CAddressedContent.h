#ifndef CADDRESSED_CONTENT_H__
#define CADDRESSED_CONTENT_H__

#include <vector>
#include <string>
#include <cstdint>
#include "SLineF.h"
#include "SRectangleF.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents an object that holds a start, end and size.
    class CAddressedContent
    {
        DECLARE_STANDARD_PROPERTY(SRectangleF, AddressStartTextArea)
        DECLARE_STANDARD_PROPERTY(std::string, AddressStartText)
        DECLARE_STANDARD_PROPERTY(SLineF, AddressStartConnectingLine)
        DECLARE_STANDARD_PROPERTY(SRectangleF, AddressEndTextArea)
        DECLARE_STANDARD_PROPERTY(std::string, AddressEndText)
        DECLARE_STANDARD_PROPERTY(SLineF, AddressEndConnctingLine)
        DECLARE_STANDARD_PROPERTY(SRectangleF, BodyArea)

    protected:
        ~CAddressedContent() = default;

    public:
        /// @brief Default constructor
        CAddressedContent() = default;
    };
}

#endif // CADDRESSED_CONTENT_H__
