#ifndef CLINKERSCRIPTSESSIONFILEINFO_H__
#define CLINKERSCRIPTSESSIONFILEINFO_H__

#include <string>

/// @brief Contains filesystem relevant information for a given session
class CLinkerScriptSessionFileInfo
{
public:
	CLinkerScriptSessionFileInfo(bool isSavedOnDisk, std::string fileName, std::string filePath)
		: m_isSavedOnDisk(isSavedOnDisk),
		m_fileName(fileName),
		m_filePath(filePath)
	{}

private:
	bool m_isSavedOnDisk;
	std::string m_fileName;
	std::string m_filePath;

public:
	bool IsSavedOnDisk() const { return this->m_isSavedOnDisk; }
	std::string FileName() const { return this->m_fileName; }
	std::string FilePath() const { return this->m_filePath; }
};

#endif