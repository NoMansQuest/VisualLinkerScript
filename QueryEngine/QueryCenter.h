#ifndef QUERYCENTER_H__
#define QUERYCENTER_H__

#include <vector>
#include <map>
#include <functional>
#include "../Models/CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CMemoryRegion.h"
#include "../Models/CLinkerScriptFile.h"
#include "../Models/CLinkerScriptContentBase.h"

namespace VisualLinkerScript::QueryEngine
{
    using namespace VisualLinkerScript::Models;

    /// @brief Queries the given object of type T from provided linker-script files
    template <typename T>
    std::multimap<std::shared_ptr<CLinkerScriptFile>, std::shared_ptr<T>> QueryObject(
        std::vector<std::shared_ptr<CLinkerScriptFile>> scope,
        std::function<bool(const CLinkerScriptFile& linkerScriptFile, T filterInput)> = nullptr);

    /// @brief Queries the given object of type T from provided linker-script file (single-file scope)
    template <typename T>
    std::multimap<std::shared_ptr<CLinkerScriptFile>, std::shared_ptr<T>> QueryObject(
        std::shared_ptr<CLinkerScriptFile> scope,
        std::function<bool(const CLinkerScriptFile& linkerScriptFile, T filterInput)> = nullptr);
}

#include "QueryCenter.tpp"

#endif // QUERYCENTER_H__
