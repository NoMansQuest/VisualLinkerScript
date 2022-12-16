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
        
         CanParseContent(const CRawEntry& startingEntry, const CRawFile& sourceRawFile);
    }
}

#endif