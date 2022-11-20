
#include "CRawFile.h"
#include <vector>
#include <memory>

using namespace VisualLinkerScript::ParsingEngine::Models;

CRawFile::CRawFile(const std::shared_ptr<std::vector<CRawEntry>> content)
{
    this->m_content = content;
}

CRawFile::~CRawFile()
{
    // No action;
}
 
std::shared_ptr<std::vector<CRawEntry>> CRawFile::getContent() const
{
    return this->m_content;
}