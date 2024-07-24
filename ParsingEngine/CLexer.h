#ifndef CPRELIMINARY_PARSER_H__
#define CPRELIMINARY_PARSER_H__

#include <vector>
#include <memory>
#include <stdexcept>
#include "Models/Raw/CRawFile.h"

using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Raw;

namespace VisualLinkerScript::ParsingEngine 
{
    /// @brief This is the preliminary parser. It breaks down the stream to 
    ///        smaller composing entries, which will then be analyzed by the main parser.
    class CLexer 
    {

    public:
        /// @brief Default constructor
        explicit CLexer() {}

        /// @brief Default destructor
        ~CLexer() {}

    public:
        /// @brief Process the input file. This function can throw an exception 
        /// @param absoluteFilePath Absolute path of the file we're processing.
        /// @param rawContent Contains the linker-script content.
        /// @exception 
        /// @return Unique pointer to the processing result in form of a CRawFile.
        std::shared_ptr<CRawFile> LexLinkerScript(std::string absoluteFilePath, std::string rawContent);
    };
}

#endif
