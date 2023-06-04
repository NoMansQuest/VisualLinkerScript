#ifndef SUBPARSER_TYPE_H__
#define SUBPARSER_TYPE_H__

#include <cstdint>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /// @brief Type of subparsers. This is to simplify
    ///        code and avoid dynamic_casts all over the place
    enum class SubParserType
    {        
        AssignmentParser,
        AssignmentProcedureParser,
        MemoryRegionParser,
        MemoryRegionContentParser,
        MemoryStatementAttributeParser,
        PhdrsRegionParser,
        PhdrsRegionContentParser,
        SectionsRegionOverlayParser,
        SectionsOutputCommandParser,
        SectionsOutputCommandContentParser,
        SectionsOutputOverlayParser,
        SectionsRegionParser,
        InputSectionParser,
        InputSectionFunctionParser,
        VersionRegionParser,
        VersionRegionContentParser,
        ExpressionParser,
        FunctionParser,
    };
};

#endif
