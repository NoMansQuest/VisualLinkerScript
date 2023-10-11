#ifndef QUERYCENTER_H__
#define QUERYCENTER_H__

#include <vector>
#include <functional>
#include "../Helpers.h"
#include "../Models/CLinkerScriptFile.h"

namespace VisualLinkerScript::QueryEngine
{
    using namespace VisualLinkerScript;
    using namespace VisualLinkerScript::Models;

    /// @brief Queries the given object of type T from provided linker-script files
    template <typename T>
    SharedPtrVector<T> QueryObject(
        const SharedPtrVector<CLinkerScriptFile>& scope,
        std::function<bool(std::shared_ptr<CLinkerScriptFile> linkerScriptFile, std::shared_ptr<T> filterInput)> filterFunction = nullptr,
        bool deepSearch = false);

    /// @brief Queries the given object of type T from provided linker-script file (single-file scope)
    template <typename T>
    SharedPtrVector<T> QueryObject(
        std::shared_ptr<CLinkerScriptFile> scope,
        std::function<bool(std::shared_ptr<CLinkerScriptFile> linkerScriptFile, std::shared_ptr<T> filterInput)> filterFunction = nullptr,
        bool deepSearch = false);
}

#include "QueryCenter.tpp"

#endif // QUERYCENTER_H__
