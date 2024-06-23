#include "QueryCenter.h"

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript::Models;

/// @brief Queries the given object of type T from provided linker-script files
template <typename T>
SharedPtrVector<QueryResult<T>> VisualLinkerScript::QueryEngine::QueryObject(
    const SharedPtrVector<CLinkerScriptFile>& scope,
    std::function<bool(std::shared_ptr<CLinkerScriptFile> linkerScriptFile, std::shared_ptr<T> filterInput)> filterFunction,
    bool deepSearch)
{

    return SharedPtrVector<QueryResult<T>>();
}

/// @brief Queries the given object of type T from provided linker-script file (single-file scope)
template <typename T>
SharedPtrVector<QueryResult<T>> VisualLinkerScript::QueryEngine::QueryObject(
    std::shared_ptr<CLinkerScriptFile> scope,
    std::function<bool(std::shared_ptr<CLinkerScriptFile> linkerScriptFile, std::shared_ptr<T> filterInput)> filterFunction,
    bool deepSearch)
{

    return SharedPtrVector<QueryResult<T>>();
}



