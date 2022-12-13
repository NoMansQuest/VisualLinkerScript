#ifndef CLINKERSCRIPTENTRYBASE_H__
#define CLINKERSCRIPTENTRYBASE_H__

#include <memory>
#include <vector>
#include "Raw/CRawEntry.h"

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

namespace VisualLinkerScript { namespace ParsingEngine { namespace Models {

/// @brief The based object shared among all elements in a Linker-Script file
class CLinkerScriptEntryBase
{

private:
    std::vector<CRawEntry> m_involvedEntries;

protected:
    CLinkerScriptEntryBase(std::vector<CRawEntry>&& involvedEntries) 
        : m_involvedEntries(std::move(involvedEntries))
    {
    }

public:
    /// @brief Returns back a list of raw entries involved in the current parsed component of 
    ///        the current linker script 
    /// @return 
    const std::vector<CRawEntry>& InvoledEntries()
    {
        return m_involvedEntries;
    }
};


}}}

#endif
