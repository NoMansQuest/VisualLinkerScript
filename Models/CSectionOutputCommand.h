#ifndef CSECTION_OUTPUT_STATEMENT_H__
#define CSECTION_OUTPUT_STATEMENT_H__

#include <vector>
#include <string>
#include "CLinkerScriptContentBase.h"
#include "Raw/CRawEntry.h"
#include "CViolationBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single 'Output' segment in the "SECTIONS"
    class CSectionOutputCommand : public CLinkerScriptContentBase
    {   
    private:
        CRawEntry m_sectionOutputNameEntry;
        SharedPtrVector<CLinkerScriptContentBase> m_preColonContent;
        SharedPtrVector<CLinkerScriptContentBase> m_postColonContent;
        SharedPtrVector<CLinkerScriptContentBase> m_innerContent;
        SharedPtrVector<CLinkerScriptContentBase> m_endingContent;
        CRawEntry m_colonEntry;        
        CRawEntry m_noCrossRefsEntry;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;


    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputCommand(const CRawEntry& sectionOutputNameEntry,
                                       const SharedPtrVector<CLinkerScriptContentBase>& preColonContent,
                                       const SharedPtrVector<CLinkerScriptContentBase>& postColonContent,
                                       const CRawEntry& colonEntry,
                                       const CRawEntry& noCrossRefsEntry,
                                       const CRawEntry& openingBracketEntry,
                                       const SharedPtrVector<CLinkerScriptContentBase>& innerContent,
                                       const CRawEntry& closingBracketEntry,
                                       const SharedPtrVector<CLinkerScriptContentBase>& endingContent,
                                       const std::vector<CRawEntry>& rawElements,
                                       const SharedPtrVector<CViolationBase>& violations)
            : CLinkerScriptContentBase(rawElements, violations),
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
        CRawEntry SectionOutputNameEntry() const
        {
            return this->m_sectionOutputNameEntry;
        }

        /// @brief Reports back the content found after the header and before the colon
        const SharedPtrVector<CLinkerScriptContentBase>& PreColonContent() const
        {
            return this->m_preColonContent;
        }

        /// @brief Reports back the content found after the colon and before the opening bracket
        const SharedPtrVector<CLinkerScriptContentBase>& PostColonContent() const
        {
            return this->m_postColonContent;
        }

        /// @brief Reports back the 'Colon' found after 'Name' and before 'Bracket-Open'.
        CRawEntry ColonEntry() const
        {
            return this->m_colonEntry;
        }

        /// @brief Reports back the 'Opening Bracket' entry, should be present.
        CRawEntry OpeningBracketEntry() const
        {
            return this->m_openingBracketEntry;
        }

        /// @brief Reports back the 'Closing Bracket' entr, should be present.
        CRawEntry ClosingBracketEntry() const
        {
            return this->m_closingBracketEntry;
        }

        /// @brief Reports back the outer content, which includes all parsed content.
        const SharedPtrVector<CLinkerScriptContentBase>& InnerContent() const
        {
            return this->m_innerContent;
        }


        /// @brief Reports back the ending content, which can contain AtLma, ToVma, Phdrs and fill-expression
        const SharedPtrVector<CLinkerScriptContentBase>& EndingContent() const
        {
            return this->m_endingContent;
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
