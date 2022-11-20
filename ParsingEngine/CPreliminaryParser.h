#ifndef CPRELIMINARY_PARSER_H__
#define CPRELIMINARY_PARSER_H__

#include <vector>
#include <memory>
#include <stdexcept>
#include "Models/CRawFile.h"

using namespace VisualLinkerScript::ParsingEngine::Models;
using namespace std;

namespace VisualLinkerScript { namespace ParsingEngine {

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
    /// @param inputStream Input stream containing the linker-script content
    /// @param length Length of the stream
    /// @exception 
    /// @return Unique pointer to the processing result in form of a CRawFile.
    std::unique_ptr<CRawFile> ProcessLinkerScript(const char* inputStream, unsigned int length);    }

}}

#endif