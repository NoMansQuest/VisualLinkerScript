#ifndef CPRELIMINARY_PARSER_H__
#define CPRELIMINARY_PARSER_H__

#include <vector>
#include <memory>
#include <stdexcept>
#include "Models/Raw/CRawFile.h"

using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace std;

namespace VisualLinkerScript::ParsingEngine 
{
    /// @brief This is the preliminary parser. It breaks down the stream to 
    ///        smaller composing entries, which will then be analyzed by the main parser.
    class CPreliminaryParser 
    {

    public:
        /// @brief Default constructor
        explicit CPreliminaryParser();

        /// @brief Default destructor
        ~CPreliminaryParser();

    public:
        /// @brief Process the input file. This function can throw an exception 
        /// @param fileName Name of the file we're processing.
        /// @param rawContent Contains the linker-script content.
        /// @exception 
        /// @return Unique pointer to the processing result in form of a CRawFile.
        std::shared_ptr<CRawFile> ProcessLinkerScript(const std::string& fileName, const std::string& rawContent);
    };
}

#endif