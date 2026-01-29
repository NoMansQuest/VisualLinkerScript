#ifndef CSECTION_OVERLAY_H__
#define CSECTION_OVERLAY_H__

#include <vector>
#include "Models/CParsedContentBase.h"

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
        LinqVector<CParsedContentBase> m_preColonContent;
        LinqVector<CParsedContentBase> m_postColonContent;
        LinqVector<CParsedContentBase> m_endingContent;
        LinqVector<CParsedContentBase> m_innerContent;

    public:
        /// @brief Default constructor, accessible to inheritors only        
        explicit CSectionOverlayCommand(const CRawEntry& headerEntry,                                        
                                        const CRawEntry& colonEntry,
                                        const CRawEntry& noCrossRefsEntry,                                        
                                        const CRawEntry& bracketOpenEntry,
                                        const CRawEntry& bracketCloseEntry,                                                                                
                                        const LinqVector<CParsedContentBase>& preColonContent,
                                        const LinqVector<CParsedContentBase>& postColonContent,
                                        const LinqVector<CParsedContentBase>& innerContent,
                                        const LinqVector<CParsedContentBase>& endingContent,
                                        const std::vector<CRawEntry>& rawElements,
                                        const LinqVector<CViolationBase>& violations)
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
        [[nodiscard]] const LinqVector<CParsedContentBase>& InnerContent() const
        {
            return this->m_innerContent;
        }

        /// @brief [Non-Optional] Reports the content defined inside between the header and colon
        [[nodiscard]] const LinqVector<CParsedContentBase>& PreColonContent() const
        {
            return this->m_preColonContent;
        }

        /// @brief [Non-Optional] Reports the content defined inside after the colon and before the bracket-open
        [[nodiscard]] const LinqVector<CParsedContentBase>& PostColonContent() const
        {
            return this->m_postColonContent;
        }

        /// @brief [Non-Optional] Reports the content defined inside after bracket-closure
        [[nodiscard]] const LinqVector<CParsedContentBase>& EndingContent() const
        {
            return this->m_endingContent;
        }

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SectionsOverlayStatement;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const LinqVector<CViolationBase> AggregateViolation() const override;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
