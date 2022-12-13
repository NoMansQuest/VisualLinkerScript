
#include "CRawFile.h"
#include <vector>
#include <memory>
#include <string.h>

using namespace VisualLinkerScript::ParsingEngine::Models::Raw;

CRawFile::CRawFile(const std::string& fileName, std::vector<CRawEntry>&& content)
    : m_content(std::move(content)),
      m_fileName(fileName)
{
}

CRawFile::~CRawFile()
{
    // No action;
}
 
const std::vector<CRawEntry>& CRawFile::Content()
{
    return this->m_content;
}
