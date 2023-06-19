#include "Helpers.h"
#include <functional>

using namespace VisualLinkerScript;

bool StringEquals(const std::string& a, const std::string& b, bool ignoreCase)
{
    std::function<bool(char,char)> caseSensitiveComparator = [=](char a, char b){ return a == b; };
    std::function<bool(char,char)> caseInsensitiveComparator = [=](char a, char b){ return tolower(a) == tolower(b); };
    auto predicate = ignoreCase ? caseInsensitiveComparator : caseSensitiveComparator;

    return a.length() == b.length() &&
           std::equal(a.begin(), a.end(),
                      b.begin(), b.end(),
                      predicate);
}
