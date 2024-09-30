#ifndef CPRELIMINARY_PARSER_H__
#define CPRELIMINARY_PARSER_H__

#include <memory>
#include "Models/Raw/CRawFile.h"

namespace VisualLinkerScript::Models
{
	class CLinkerScriptFile;
}

namespace VisualLinkerScript::ParsingEngine 
{
    using namespace VisualLinkerScript::Models;
    using namespace VisualLinkerScript::Models::Raw;

    /// @brief This is the preliminary parser. It breaks down the stream to 
    ///        smaller composing entries, which will then be analyzed by the main parser.
    class CLinkerScriptLexer 
    {
    public:
        /// @brief Process the input file. This function can throw an exception 
        /// @param linkerScriptFile The linker script file.
        /// @exception
        static void LexLinkerScript(std::shared_ptr<CLinkerScriptFile>& linkerScriptFile);
    };
}

#endif
