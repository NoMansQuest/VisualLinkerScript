#ifndef CSECTION_OUTPUT_OVERLAY_H__
#define CSECTION_OUTPUT_OVERLAY_H__

#include <vector>
#include <memory>
#include <string>
#include "CSectionOutputToVmaRegion.h"
#include "CSectionOutputAtLmaRegion.h"
#include "CSectionOutputPhdr.h"
#include "CSectionOutputFillExpression.h"
#include "Raw/CRawEntry.h"
#include "CParserViolation.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single 'OVERLAY' command found under "SECTIONS"
    class CSectionOutputOverlay : public CLinkerScriptContentBase
    {
    private:
        CRawEntry m_sectionOutputNameEntry;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_preColonContent;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_postColonContent;
        CRawEntry m_colonEntry;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;
        std::shared_ptr<CSectionOutputToVmaRegion> m_toVmaRegion;
        std::shared_ptr<CSectionOutputAtLmaRegion> m_atLmaRegionFunction;
        std::vector<std::shared_ptr<CSectionOutputPhdr>> m_programHeaders;
        std::shared_ptr<CSectionOutputFillExpression> m_fillExpression;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_innerContent;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CSectionOutputOverlay(CRawEntry sectionOutputNameEntry,
                                       std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& preColonContent,
                                       std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& postColonContent,
                                       CRawEntry colonEntry,
                                       CRawEntry openingBracketEntry,
                                       CRawEntry closingBracketEntry,
                                       std::shared_ptr<CSectionOutputToVmaRegion> toVmaRegion,
                                       std::shared_ptr<CSectionOutputAtLmaRegion> atLmaRegionFunction,
                                       std::vector<std::shared_ptr<CSectionOutputPhdr>>&& programHeaders,
                                       std::shared_ptr<CSectionOutputFillExpression> fillExpression,
                                       std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& innerContent,
                                       std::vector<CRawEntry>&& rawElements,
                                       std::vector<CParserViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_sectionOutputNameEntry(sectionOutputNameEntry),
              m_preColonContent(std::move(preColonContent)),
              m_postColonContent(std::move(postColonContent)),
              m_colonEntry(colonEntry),
              m_openingBracketEntry(openingBracketEntry),
              m_closingBracketEntry(closingBracketEntry),
              m_toVmaRegion(toVmaRegion),
              m_atLmaRegionFunction(atLmaRegionFunction),
              m_programHeaders(std::move(programHeaders)),
              m_fillExpression(fillExpression),
              m_innerContent(std::move(innerContent))
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionsOverlayStatement;
        }

        /// @brief Reports back the name of the SectionOutput
        CRawEntry SectionOutputNameEntry()
        {
            return this->m_sectionOutputNameEntry;
        }

        /// @brief Reports back the content found after the header and before the colon
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& PreColonContent()
        {
            return this->m_preColonContent;
        }

        /// @brief Reports back the content found after the colon and before the opening bracket
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& PostColonContent()
        {
            return this->m_postColonContent;
        }

        /// @brief Reports back the 'Colon' found after 'Name' and before 'Bracket-Open'.
        CRawEntry ColonEntry()
        {
            return this->m_colonEntry;
        }

        /// @brief Reports back the 'Opening Bracket' entry, should be present.
        CRawEntry OpeningBracketEntry()
        {
            return this->m_openingBracketEntry;
        }

        /// @brief Reports back the 'Closing Bracket' entr, should be present.
        CRawEntry ClosingBracketEntry()
        {
            return this->m_closingBracketEntry;
        }

        /// @brief Reports back the 'ToVma' expression, if present.
        std::shared_ptr<CSectionOutputToVmaRegion> ToVmaRegion()
        {
            return thi   
s->m_toVmaRegion;
        }

        /// @brief Reports back the 'AtLma' region, if present.
        std::shared_ptr<CSectionOutputAtLmaRegion> AtLmaRegionFunction()
        {
            return this->m_atLmaRegionFunction;
        }

        /// @brief Reports back the program headers defined, if present.
        const std::vector<std::shared_ptr<CSectionOutputPhdr>>& ProgramHeaders()
        {
            return this->m_programHeaders;
        }

        /// @brief Reports back the 'Fill' expression, if preent.
        std::shared_ptr<CSectionOutputFillExpression> FillExpression()
        {
            return this->m_fillExpression;
        }

        /// @brief Reports back the outer content, which includes all parsed content.
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& InnerContent()
        {
            return this->m_innerContent;
        }
    };
}

#endif
