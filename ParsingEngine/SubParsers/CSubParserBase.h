#ifndef CSUBPARSER_BASE_H__
#define CSUBPARSER_BASE_H__

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    using namespace VisualLinkerScript::ParsingEngine::Models::Raw;    

    /// @brief Base class for all sub-parsers
    class CSubParserBase
    {
    public:
        /// @brief Default constructor
        CSubParserBase();

        /// @brief Desctructor
        ~CSubParserBase();

    public:
        /// @brief Try to parse the content. 
        /// @param currentPosition [In] Position to start parser at. 
        /// @param sourceRawFile The CRawFile        
        /// @return False if parsing failed. True if parser successful, in which case 'currentPosition' is updated accordingly.
        bool TryParse(uint32_t parseStartPosition, const CRawFile& rawFile, std::vector<) = 0;
    }
}

#endif