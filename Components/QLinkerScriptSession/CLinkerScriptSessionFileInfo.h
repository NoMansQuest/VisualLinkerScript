#ifndef CLINKERSCRIPTSESSIONFILEINFO_H__
#define CLINKERSCRIPTSESSIONFILEINFO_H__

#include <string>

/// @brief Contains filesystem relevant information for a given session
class CLinkerScriptSessionFileInfo
{
public:
	CLinkerScriptSessionFileInfo(
		const bool isSavedOnDisk,
		const std::string& absoluteFilePath,
		const std::string& fileName,
		const std::string& filePath,
		const std::string& onDiskSignature)
		: m_isSavedOnDisk(isSavedOnDisk),
		m_fileName(fileName),
		m_filePath(filePath),
		m_absoluteFilePath(absoluteFilePath),
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