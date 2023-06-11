#ifndef CACTIONBASE_H__
#define CACTIONBASE_H__

#include <cstdint>
#include <string>

namespace VisualLinkerScript::Models::Intervention
{
    /// @brief Describes text location and text to insert
    class CActionBase
    {
    private:
        uint32_t m_startLocation;

    public:
        /// @brief Default constructor
        CActionBase(uint32_t startLocation)
            : m_startLocation(startLocation)
        {}

        /// @brief Default desconstructor
        ~CActionBase() {}

    public:
        /// @brief Location of the first character to remove
        uint32_t StartLocation() {
            return m_startLocation;
        }
    };
}


#endif // CACTIONBASE_H__
