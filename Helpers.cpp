#include "Helpers.h"
#include <functional>
#include <QFile>
#include <qguiapplication.h>
#include <string>
#include <QTextStream>

using namespace VisualLinkerScript;


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
    return false;
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

bool VisualLinkerScript::ReadFileContent(const std::string& filePath, std::string& fileContent, std::string& errorMessage)
{
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorMessage = "Error", "Cannot open file for reading";
        return false;
    }

    QTextStream in(&file);
    fileContent = in.readAll().toStdString();
    file.close();
    return true;
}

qreal Graphical::GetFontSizeFromMetric(const double dpiY, const double desiredHeightMilliMeters)
{
    const double heightInInches = desiredHeightMilliMeters / 25.4; // 1 inch = 25.4 mm
    const double heightInPixels = heightInInches * dpiY;
    return heightInPixels * 72 / dpiY;
}

qreal Graphical::GetPixelsInMetric(const double dpi, const double desiredSizeInMilliMeters)
{    
    return (dpi / 25.4) * desiredSizeInMilliMeters;
}

qreal Graphical::GetMetricFromPixels(const double dpi, const double desiredSizeInPixels)
{
    return (desiredSizeInPixels / dpi) * 25.4;
}

qreal Graphical::GetTextWidthInPixels(const std::string& stringToMeasure, const QFontMetrics& fontMetrics)
{    
    return fontMetrics.horizontalAdvance(QString::fromStdString(stringToMeasure));
}

std::string VisualLinkerScript::StringLTrim(const std::string& sourceString)
{
    auto start = std::find_if_not(sourceString.begin(), sourceString.end(), [](unsigned char ch) {
        return std::isspace(ch);
        });

    return std::string(start, sourceString.end());
}

std::string VisualLinkerScript::StringRTrim(const std::string& sourceString)
{
    auto end = std::find_if_not(sourceString.rbegin(), sourceString.rend(), [](unsigned char ch) {
        return std::isspace(ch);
        });

    return std::string(sourceString.begin(), end.base());
}


std::string VisualLinkerScript::StringTrim(const std::string& sourceString)
{
    return StringRTrim(StringLTrim(sourceString));
}

