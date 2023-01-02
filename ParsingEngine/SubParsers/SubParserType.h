#ifndef SUBPARSER_TYPE_H__
#define SUBPARSER_TYPE_H__

#include <cstdint>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    enum class SubParserType : uint32_t
    {        
        AssignmentParser,
        DefaultParser,
        MemoryRegionParser,
        PhdrsRegionParser,
        PhdrsRegionContentParser
        SectionsRegionOverlayParser,
        SectionsRegionParser,
        VersionRegionParser,
        ExpressionParser
    };
}

#endif