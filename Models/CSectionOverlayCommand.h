#ifndef CSECTION_OVERLAY_H__
#define CSECTION_OVERLAY_H__

#include <vector>
#include "CParsedContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single 'Output' segment in the "SECTIONS"
    class CSectionOverlayCommand : public CParsedContentBase
    {   
    private:
        CRawEntry m_headerEntry;        
        CRawEntry m_colonEntry;
        CRawEntry m_noCrossRefsEntry;        
        CRawEntry m_bracketOpenEntry;
        CRawEntry m_bracketCloseEntry;                
        SharedPtrVector<CParsedContentBase> m_preColonContent;
        SharedPtrVector<CParsedContentBase> m_postColonContent;
        SharedPtrVector<CParsedContentBase> m_endingContent;
        SharedPtrVector<CParsedContentBase> m_innerContent;

    public:
        /// @brief Default constructor, accessible to inheritors only        
        explicit CSectionOverlayCommand(const CRawEntry& headerEntry,                                        
                                        const CRawEntry& colonEntry,
                                        const CRawEntry& noCrossRefsEntry,                                        
                                        const CRawEntry& bracketOpenEntry,
                                        const CRawEntry& bracketCloseEntry,                                                                                
                                        const SharedPtrVector<CParsedContentBase>& preColonContent,
                                        const SharedPtrVector<CParsedContentBase>& postColonContent,
                                        const SharedPtrVector<CParsedContentBase>& innerContent,
                                        const SharedPtrVector<CParsedContentBase>& endingContent,
                                        const std::vector<CRawEntry>& rawElements,
                                        const SharedPtrVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_headerEntry(headerEntry),              
              m_colonEntry(colonEntry),
              m_noCrossRefsEntry(noCrossRefsEntry),              
              m_bracketOpenEntry(bracketOpenEntry),
              m_bracketCloseEntry(bracketCloseEntry),
              m_preColonContent(preColonContent),
              m_postColonContent(postColonContent),
              m_endingContent(endingContent),
              m_innerContent(innerContent)
        {}        

    public:

        /// @brief [Non-Optional] Header entry (i.e. 'OVERLAY').
        [[nodiscard]] CRawEntry HeaderEntry() const
        {
            return this->m_headerEntry;
        }

        /// @brief [Non-Optional] Reports back the 'Colon' entry.
        [[nodiscard]] CRawEntry ColonEntry() const
        {
            return this->m_colonEntry;
        }

        /// @brief [Non-Optional] Reports the curly-bracket open entry.
        [[nodiscard]] CRawEntry BracketOpenEntry() const
        {
            return this->m_bracketOpenEntry;
        }

        /// @brief [Non-Optional] Reports the curly-bracket close entry.
        [[nodiscard]] CRawEntry BracketCloseEntry() const
        {
            return this->m_bracketCloseEntry;
        }

        /// @brief [Non-Optional] Reports the content defined inside the 'OVERLAY'
        [[nodiscard]] const SharedPtrVector<CParsedContentBase>& InnerContent() const
        {
            return this->m_innerContent;
        }

        /// @brief [Non-Optional] Reports the content defined inside between the header and colon
        [[nodiscard]] const SharedPtrVector<CParsedContentBase>& PreColonContent() const
        {
            return this->m_preColonContent;
        }

        /// @brief [Non-Optional] Reports the content defined inside after the colon and before the bracket-open
        [[nodiscard]] const SharedPtrVector<CParsedContentBase>& PostColonContent() const
        {
            return this->m_postColonContent;
        }

        /// @brief [Non-Optional] Reports the content defined inside after bracket-closure
        [[nodiscard]] const SharedPtrVector<CParsedContentBase>& EndingContent() const
        {
            return this->m_endingContent;
        }

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SectionsOverlayStatement;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const SharedPtrVector<CViolationBase> AggregateViolation() const override;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
