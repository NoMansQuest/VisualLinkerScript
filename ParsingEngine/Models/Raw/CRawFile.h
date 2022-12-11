#ifndef CRAWFILE_H__
#define CRAWFILE_H__

#include <vector>
#include <memory>
#include <string>
#include "CRawEntry.h"

namespace VisualLinkerScript { namespace ParsingEngine { namespace Models {

    /// @brief This object represents a file processed by @see CPreliminaryParser
    class CRawFile
    {
    public:
        /// @brief Default constructor
        /// @param content Vector of CRawEntry, which the linker-script is composed of.
        explicit CRawFile(const std::string& fileName, std::shared_ptr<std::vector<CRawEntry>> content);

        /// @brief Default destructor
        ~CRawFile();

    private:         
        std::shared_ptr<std::vector<CRawEntry>> m_content;
        std::string m_fileName;

    public:
        /// @brief Retried the content's vector        
        std::shared_ptr<std::vector<CRawEntry>> Content() const;

        /// @brief Name of the loaded file        
        std::string FileName() const;
    };

}}}

#endif
