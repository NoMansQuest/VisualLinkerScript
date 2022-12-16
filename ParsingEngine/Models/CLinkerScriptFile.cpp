#include "CLinkerScriptFile.h"
#include "CLinkerScriptContentBase.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace VisualLinkerScript::ParsingEngine::Models;

const std::vector<CLinkerScriptContentBase>& CLinkerScriptFile::Content()
{
    return m_content;
}

const std::string& CLinkerScriptFile::RawText()
{
    return m_rawText;
}

const std::string CLinkerScriptFile::ResolveEntryText(CLinkerScriptContentBase& entryToResolve)
{
    auto startingPosition = entryToResolve.InvoledEntries().front().StartPosition();
    auto lastElementInVector = entryToResolve.InvoledEntries().back();
    auto endPosition = lastElementInVector.StartPosition() + lastElementInVector.Length();
    return this->m_rawText.substr(startingPosition, endPosition - startingPosition + 1);
}
