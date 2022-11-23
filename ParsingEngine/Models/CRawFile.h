#ifndef CRAWFILE_H__
#define CRAWFILE_H__

#include <vector>
#include <memory>
#include "CRawEntry.h"

namespace VisualLinkerScript { namespace ParsingEngine { namespace Models {

    /// @brief This object represents a file processed by @see CPreliminaryParser
    class CRawFile
    {
    public:
        /// @brief Default constructor
        /// @param content Vector of CRawEntry, which the linker-script is composed of.
        explicit CRawFile(std::shared_ptr<std::vector<CRawEntry>> content);

        /// @brief Default destructor
        ~CRawFile();

    private:
        std::shared_ptr<std::vector<CRawEntry>> m_content;

    public:
        /// @brief Retried the content's vector
        /// @return Shared pointer to the vector of content.
        std::shared_ptr<std::vector<CRawEntry>> getContent() const;
    };

}}}

#endif
