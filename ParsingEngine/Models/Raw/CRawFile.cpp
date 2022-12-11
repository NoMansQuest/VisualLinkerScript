
#include "CRawFile.h"
#include <vector>
#include <memory>
#include <string.h>

using namespace VisualLinkerScript::ParsingEngine::Models;

CRawFile::CRawFile(const std::string& fileName, const std::shared_ptr<std::vector<CRawEntry>> content)
{
    this->m_fileName = fileName; 
    this->m_content = content;
}

CRawFile::~CRawFile()
{
    // No action;
}
 
std::shared_ptr<std::vector<CRawEntry>> CRawFile::Content() const
{
    return this->m_content;
}
