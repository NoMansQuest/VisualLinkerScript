#include "CLinkerScriptFile.h"
#include "CLinkerScriptContentBase.h"

using namespace VisualLinkerScript::Models;

/// @brief Returns the full text the input component is composed of.
/// @param entryToResolve Component to process
/// @return The full text that constitutes the component.
const std::string CLinkerScriptFile::ResolveEntryText(CLinkerScriptContentBase& contentToResolve) const {
    auto startPosition = contentToResolve.RawEntries().front().StartPosition();
    auto endPosition = contentToResolve.RawEntries().back().StartPosition() +
                       contentToResolve.RawEntries().back().Length() - 1;

    return this->m_rawFile->FileContent().substr(startPosition, endPosition - startPosition + 1);
}

/// @brief Returns the full text the input component is composed of.
/// @param entryToResolve Component to process
/// @return The full text that constitutes the component.
const std::string CLinkerScriptFile::ResolveEntryText(const CRawEntry& rawEntryToResolve) const {
    return this->m_rawFile->FileContent().substr(
                rawEntryToResolve.StartPosition(),
                rawEntryToResolve.Length());
}
