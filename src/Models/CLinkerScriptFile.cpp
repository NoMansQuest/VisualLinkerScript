#include "Models/CLinkerScriptFile.h"
#include "Models/CParsedContentBase.h"
#include <string>

using namespace VisualLinkerScript::Models;

std::string CLinkerScriptFile::ToDebugInfo(uint32_t depth) const
{
    std::string content = " LinkerScriptFile: " + this->FileName() + "\n";
    content += std::string(depth, ' ') + " -- content : \n";

    for (const auto& subContent : this->ParsedContent())
    {
        content += subContent->ToDebugInfo(depth + 4, *this) + "\n";
    }

    return std::string(depth, ' ') + " - " + content;
}
