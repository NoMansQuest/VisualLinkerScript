#ifndef CBACKEND_RESPONSE_H__
#define CBACKEND_RESPONSE_H__

#include <string>

namespace VisualLinkerScript::Components::MemoryVisualizer
{
    /// @brief Filled by backend upon once it receives certain signals
    struct SBackendResponse
    {
        /// @brief If true, then backend has accepted users request. The view will be rebuilt short.
        bool accepted;

        /// @brief If request has been rejected by the backend (i.e. @see {accepted} being 'false'), then
        ///        this message will be shown to the user.
        std::string rejectionReason;
    };
};

#endif // CBACKEND_RESPONSE_H__
