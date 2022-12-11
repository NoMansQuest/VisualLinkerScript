#ifndef CLINKERSCRIPTFILE_H__
#define CLINKERSCRIPTFILE_H__

#include <memory>
#include <vector>
#include "CLinkerScriptEntryBase.h"

namespace VisualLinkerScript { namespace ParsingEngine { namespace Models {

/// @brief Object representing a parsed Linker-Script
class CLinkerScriptFile
{

public:
    /// @brief Default constructor
    explicit CLinkerScriptFile(std::shared_ptr<std::vector<CLinkerScriptEntryBase>> content);

    /// @brief Default destructor
    ~CLinkerScriptFile();

private:



};

}}}

#endif
