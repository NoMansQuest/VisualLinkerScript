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

    /// @brief Query returns these objects to report findings and belonging linker-script file.
    template<typename TObjectType>
    struct QueryResult
    {
        private:
            std::shared_ptr<CLinkerScriptFile> m_linkerScriptFile;
            std::shared_ptr<TObjectType> m_result;

        public:
            QueryResult(std::shared_ptr<CLinkerScriptFile> linkerScriptFile, std::shared_ptr<TObjectType> result)
            {
                this->m_result = result;
                this->m_linkerScriptFile = linkerScriptFile;
            }

        public:
            std::shared_ptr<CLinkerScriptFile> LinkerScriptFile() { return this->m_linkerScriptFile; }
            std::shared_ptr<TObjectType> Result() { return this->m_result; }
    };

    /// @brief Queries the given object of type T from provided linker-script files
    template <typename T>
    LinqVector<QueryResult<T>> QueryObject(
        const LinqVector<CLinkerScriptFile>& scope,
        std::function<bool(std::shared_ptr<CLinkerScriptFile> linkerScriptFile, std::shared_ptr<T> filterInput)> filterFunction = nullptr,
        bool deepSearch = false);

    /// @brief Queries the given object of type T from provided linker-script file (single-file scope)
    template <typename T>
    LinqVector<QueryResult<T>> QueryObject(
        std::shared_ptr<CLinkerScriptFile> scope,
        std::function<bool(std::shared_ptr<CLinkerScriptFile> linkerScriptFile, std::shared_ptr<T> filterInput)> filterFunction = nullptr,
        bool deepSearch = false);
}

#include "QueryCenter.tpp"

#endif // QUERYCENTER_H__
