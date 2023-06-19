#include "QueryCenter.h"

using namespace VisualLinkerScript::QueryEngine;
using namespace VisualLinkerScript::Models;

template <typename T>
std::multimap<std::shared_ptr<CLinkerScriptFile>, std::shared_ptr<T>> QueryObject(
    std::vector<std::shared_ptr<CLinkerScriptFile>> scope,
    std::function<bool(const CLinkerScriptFile& linkerScriptFile, T filterInput)> filter)
{

}



