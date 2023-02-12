#ifndef CMASTER_PARSER_H__
#define CMASTER_PARSER_H__

#include "Models/Raw/CRawFile.h"
#include "Models/CLinkerScriptFile.h"
#include <vector>
#include <memory>

using namespace VisualLinkerScript::ParsingEngine::Models;

namespace VisualLinkerScript::ParsingEngine
{
    /// @brief The main parser, in charge of parsing a give linker-script file
    class CMasterParser
    {
    public:
        /// @brief Default constructor
        explicit CMasterParser()
        {}

        /// @brief Processes a @see {CRawFile} and produces a @see {CLinkerScriptFile}
        CLinkerScriptFile&& ProcessLinkerScriptFile(std::shared_ptr<CRawFile> rawFile);
    };
}

#endif
