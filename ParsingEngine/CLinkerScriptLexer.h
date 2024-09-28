#ifndef CPRELIMINARY_PARSER_H__
#define CPRELIMINARY_PARSER_H__

#include <vector>
#include <memory>
#include <stdexcept>
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
        /// @param filePath Absolute path of the file we're processing.
        /// @param fileContent Contains the linker-script content.
        /// @exception 
        /// @return Unique pointer to the processing result in form of a CRawFile.
        static std::shared_ptr<CLinkerScriptFile> LexLinkerScript(const std::string& filePath, std::string fileContent);
    };
}

#endif
