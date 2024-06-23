#ifndef CLINKERSCRIPTSESSIONFILEINFO_H__
#define CLINKERSCRIPTSESSIONFILEINFO_H__

#include <string>

/// @brief Contains filesystem relevant information for a given session
class CLinkerScriptSessionFileInfo
{
public:
	CLinkerScriptSessionFileInfo(
		bool isSavedOnDisk,
		std::string absoluteFilePath,
		std::string fileName, 
		std::string filePath,
		std::string onDiskSignature)
		: m_isSavedOnDisk(isSavedOnDisk),
		m_fileName(fileName),
		m_filePath(filePath),
		m_onDiskSignature(onDiskSignature)
	{}

private:
	bool m_isSavedOnDisk;
	std::string m_fileName;
	std::string m_filePath;
	std::string m_absoluteFilePath;
	std::string m_onDiskSignature;

public:
	bool IsSavedOnDisk() const			 { return this->m_isSavedOnDisk; }
	std::string AbsoluteFilePath() const { return this->m_absoluteFilePath; }
	std::string FileName() const		 { return this->m_fileName; }
	std::string FilePath() const		 { return this->m_filePath; }
	std::string OnDiskSignature() const	 { return this->m_onDiskSignature; }
};

#endif