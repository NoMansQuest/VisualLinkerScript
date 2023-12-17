#ifndef CSECTION_OUTPUT_STATEMENT_H__
#define CSECTION_OUTPUT_STATEMENT_H__

#include <vector>
#include <memory>
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
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_preColonContent;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_postColonContent;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_innerContent;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_endingContent;
        CRawEntry m_colonEntry;        
        CRawEntry m_noCrossRefsEntry;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;


    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CSectionOutputCommand(CRawEntry sectionOutputNameEntry,
                                       std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& preColonContent,
                                       std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& postColonContent,
                                       CRawEntry colonEntry,
                                       CRawEntry noCrossRefsEntry,
                                       CRawEntry openingBracketEntry,
                                       std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& innerContent,
                                       CRawEntry closingBracketEntry,
                                       std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& endingContent,
                                       std::vector<CRawEntry>&& rawElements,
                                       SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_sectionOutputNameEntry(sectionOutputNameEntry),
              m_preColonContent(std::move(preColonContent)),
              m_postColonContent(std::move(postColonContent)),
              m_innerContent(std::move(innerContent)),
              m_endingContent(std::move(endingContent)),
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
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& PreColonContent() const
        {
            return this->m_preColonContent;
        }

        /// @brief Reports back the content found after the colon and before the opening bracket
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& PostColonContent() const
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
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& InnerContent() const
        {
            return this->m_innerContent;
        }


        /// @brief Reports back the ending content, which can contain AtLma, ToVma, Phdrs and fill-expression
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& EndingContent() const
        {
            return this->m_endingContent;
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
