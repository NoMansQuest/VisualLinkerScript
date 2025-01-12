#ifndef INDENTATION_INFO_H_
#define INDENTATION_INFO_H_

#include <vector>

namespace VisualLinkerScript::Models
{
    // @brief We support multiple types of indentation
    enum class EIndentationLineType
    {
	    RegularSpace,
        MultiLineCommentStar,
        RegularCommentDoubleSlash
    };

    /// @brief Represents a comment entry
    class CIndentationInfo
    {
        uint32_t m_indentationDepth;
        EIndentationLineType m_indentationType;
        bool m_commentLinePrefixing;

    public:

        /// @brief Default constructor introduced for compatibility with
        ///        STL containers. 
        explicit CIndentationInfo():
			m_indentationDepth(0),
			m_indentationType(EIndentationLineType::RegularSpace),
			m_commentLinePrefixing(false)
        {   
        }

        /// @brief Default constructor, accessible to inheritors only
        /// @param indentationDepth Indentation depth
        /// @param indentationType Indentation type (usually means prefix)
        /// @param commentLinePrefixing Indicates whether a star has to be added to the new line since it's a comment.
        explicit CIndentationInfo(
            const uint32_t indentationDepth, 
            const EIndentationLineType indentationType,
            const bool commentLinePrefixing)
        {
            this->m_indentationDepth = indentationDepth;
            this->m_indentationType = indentationType;
            this->m_commentLinePrefixing = commentLinePrefixing;
        }

        /// @brief Reports back the depth of indentation.
        [[nodiscard]] uint32_t IndentationDepth() const { return this->m_indentationDepth; }

        /// @brief Reports back the type of indentation.
        [[nodiscard]] EIndentationLineType IndentationType() const { return this->m_indentationType;  }

        /// @brief Reports back the type of indentation.
        [[nodiscard]] bool CommentLinePrefixing() const { return this->m_commentLinePrefixing; }

    };
}


#endif

