#ifndef CSECTION_OUTPUT_STATEMENT_H__
#define CSECTION_OUTPUT_STATEMENT_H__

#include <vector>
#include <string>
#include "CParsedContentBase.h"
#include "Raw/CRawEntry.h"
#include "CViolationBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single 'Output' segment in the "SECTIONS"
    class CSectionOutputCommand : public CParsedContentBase
    {   
    private:
        CRawEntry m_sectionOutputNameEntry;
        SharedPtrVector<CParsedContentBase> m_preColonContent;
        SharedPtrVector<CParsedContentBase> m_postColonContent;
        SharedPtrVector<CParsedContentBase> m_innerContent;
        SharedPtrVector<CParsedContentBase> m_endingContent;
        CRawEntry m_colonEntry;        
        CRawEntry m_noCrossRefsEntry;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;


    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputCommand(const CRawEntry& sectionOutputNameEntry,
                                       const SharedPtrVector<CParsedContentBase>& preColonContent,
                                       const SharedPtrVector<CParsedContentBase>& postColonContent,
                                       const CRawEntry& colonEntry,
                                       const CRawEntry& noCrossRefsEntry,
                                       const CRawEntry& openingBracketEntry,
                                       const SharedPtrVector<CParsedContentBase>& innerContent,
                                       const CRawEntry& closingBracketEntry,
                                       const SharedPtrVector<CParsedContentBase>& endingContent,
                                       const std::vector<CRawEntry>& rawElements,
                                       const SharedPtrVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_sectionOutputNameEntry(sectionOutputNameEntry),
              m_preColonContent(preColonContent),
              m_postColonContent(postColonContent),
              m_innerContent(innerContent),
              m_endingContent(endingContent),
              m_colonEntry(colonEntry),
              m_noCrossRefsEntry(noCrossRefsEntry),
              m_openingBracketEntry(openingBracketEntry),
              m_closingBracketEntry(closingBracketEntry)
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SectionsOutputCommand;
        }

        /// @brief Reports back the name of the SectionOutput
        [[nodiscard]] CRawEntry SectionOutputNameEntry() const
        {
            return this->m_sectionOutputNameEntry;
        }

        /// @brief Reports back the content found after the header and before the colon
        [[nodiscard]] const SharedPtrVector<CParsedContentBase>& PreColonContent() const
        {
            return this->m_preColonContent;
        }

        /// @brief Reports back the content found after the colon and before the opening bracket
        [[nodiscard]] const SharedPtrVector<CParsedContentBase>& PostColonContent() const
        {
            return this->m_postColonContent;
        }

        /// @brief Reports back the 'Colon' found after 'Name' and before 'Bracket-Open'.
        [[nodiscard]] CRawEntry ColonEntry() const
        {
            return this->m_colonEntry;
        }

        /// @brief Reports back the 'Opening Bracket' entry, should be present.
        [[nodiscard]] CRawEntry OpeningBracketEntry() const
        {
            return this->m_openingBracketEntry;
        }

        /// @brief Reports back the 'Closing Bracket' entr, should be present.
        [[nodiscard]] CRawEntry ClosingBracketEntry() const
        {
            return this->m_closingBracketEntry;
        }

        /// @brief Reports back the outer content, which includes all parsed content.
        [[nodiscard]] const SharedPtrVector<CParsedContentBase>& InnerContent() const
        {
            return this->m_innerContent;
        }

        /// @brief Reports back the ending content, which can contain AtLma, ToVma, Phdrs and fill-expression
        [[nodiscard]] const SharedPtrVector<CParsedContentBase>& EndingContent() const
        {
            return this->m_endingContent;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const SharedPtrVector<CViolationBase> AggregateViolation() const override;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
