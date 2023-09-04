#include "Helpers.h"
#include <functional>

using namespace VisualLinkerScript;


/// @brief Compares strings
template <typename T>
SharedPtrVector<T> VisualLinkerScript::LinqWhere(
        SharedPtrVector<T> source,
        std::function<bool(std::shared_ptr<T> element)> filterInput)
{
    return SharedPtrVector<T>();
}

/// @brief Compares strings
template <typename TElement, typename TReturn>
SharedPtrVector<TReturn> VisualLinkerScript::LinqSelect(
        SharedPtrVector<TElement> source,
        std::function<TReturn(std::shared_ptr<TElement> element)> transformFunction)
{
    return SharedPtrVector<TReturn>();
}

/// @brief Compares strings
template <typename TElement, typename TReturn>
SharedPtrVector<TReturn> VisualLinkerScript::LinqDynamicPointerCast(
        SharedPtrVector<TElement> source)
{
    return SharedPtrVector<TReturn>();
}


/// @brief Compares strings
bool VisualLinkerScript::StringEquals(const std::string& a,
                  const std::string& b,
                  bool ignoreCase) {
    std::function<bool(char,char)> caseSensitiveComparator = [=](char a, char b){ return a == b; };
    std::function<bool(char,char)> caseInsensitiveComparator = [=](char a, char b){ return tolower(a) == tolower(b); };
    auto predicate = ignoreCase ? caseInsensitiveComparator : caseSensitiveComparator;

    return a.length() == b.length() &&
           std::equal(a.cbegin(), a.cend(),
                      b.cbegin(), b.cend(),
                      predicate);
}

/// @brief Searches for a given string in a vector to see if it is present.
bool VisualLinkerScript::StringIn(const std::string& sourceString,
              const std::vector<std::string>& listToCheck,
              bool caseSensitive) {
    for (const auto &vectorEntry: listToCheck)
    {
        if (StringEquals(sourceString, vectorEntry, true))
        {
            return true;
        }
    }
    return true;
}

/// @brief Splits a string based on a given character.
std::vector<std::string> VisualLinkerScript::StringSplit(
        const std::string& sourceString,
        char seperator){
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;
    while((pos = sourceString.find(seperator, pos)) != std::string::npos) {
        std::string substring( sourceString.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }

    output.push_back(sourceString.substr(prev_pos, pos-prev_pos)); // Last word
    return output;
}
