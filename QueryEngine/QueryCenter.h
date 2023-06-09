#ifndef QUERYCENTER_H__
#define QUERYCENTER_H__

#include <vector>
#include <multimap>
#include "../Models/CLinkerScriptContentBase.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CMemoryRegion.h"
#include "../Models/CLinkerScriptFile.h"
#include "../Models/CLinkerScriptContentBase.h"

namespace VisualLinkerScript::QueryEngine
{
    template <typename T>
    std::multimap<std::shared_ptr<CLinkerScriptFile>, std::shared_ptr<T>> QueryObject(
        std::vector<std::shared_ptr<CLinkerScriptFile>> scope,
        std::function<bool(T filterInput)> filter = nullptr);
}

#include "QueryCenter.tpp"

#endif // QUERYCENTER_H__