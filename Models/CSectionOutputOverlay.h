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
#include "CViolationBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single 'OVERLAY' command found under "SECTIONS"
    class CSectionOutputOverlay : public CParsedContentBase
    {
    private:
        CRawEntry m_sectionOutputNameEntry;
        SharedPtrVector<CParsedContentBase> m_preColonContent;
        SharedPtrVector<CParsedContentBase> m_postColonContent;
        CRawEntry m_colonEntry;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;
        std::shared_ptr<CSectionOutputToVmaRegion> m_toVmaRegion;
        std::shared_ptr<CSectionOutputAtLmaRegion> m_atLmaRegionFunction;
        SharedPtrVector<CSectionOutputPhdr> m_programHeaders;
        std::shared_ptr<CSectionOutputFillExpression> m_fillExpression;
        SharedPtrVector<CParsedContentBase> m_innerContent;

    public:
        /// @brief Default constructor, accessible to inheritors only
        explicit CSectionOutputOverlay(const CRawEntry& sectionOutputNameEntry,
                                       const SharedPtrVector<CParsedContentBase>& preColonContent,
                                       const SharedPtrVector<CParsedContentBase>& postColonContent,
                                       const CRawEntry& colonEntry,
                                       const CRawEntry& openingBracketEntry,
                                       const CRawEntry& closingBracketEntry,
                                       const std::shared_ptr<CSectionOutputToVmaRegion>& toVmaRegion,
                                       const std::shared_ptr<CSectionOutputAtLmaRegion>& atLmaRegionFunction,
                                       const SharedPtrVector<CSectionOutputPhdr>& programHeaders,
                                       const std::shared_ptr<CSectionOutputFillExpression>& fillExpression,
                                       const SharedPtrVector<CParsedContentBase>& innerContent,
                                       const std::vector<CRawEntry>& rawElements,
                                       const SharedPtrVector<CViolationBase>& violations)
            : CParsedContentBase(rawElements, violations),
              m_sectionOutputNameEntry(sectionOutputNameEntry),
              m_preColonContent(preColonContent),
              m_postColonContent(postColonContent),
              m_colonEntry(colonEntry),
              m_openingBracketEntry(openingBracketEntry),
              m_closingBracketEntry(closingBracketEntry),
              m_toVmaRegion(toVmaRegion),
              m_atLmaRegionFunction(atLmaRegionFunction),
              m_programHeaders(programHeaders),
              m_fillExpression(fillExpression),
              m_innerContent(innerContent)
        {}

    public:
        /// @brief Reports back the type of this object.
        ContentType Type() override
        {
            return ContentType::SectionsOverlayStatement;
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

        /// @brief Reports back the 'ToVma' expression, if present.
        [[nodiscard]] const std::shared_ptr<CSectionOutputToVmaRegion>& ToVmaRegion() const
        {
            return this->m_toVmaRegion;
        }

        /// @brief Reports back the 'AtLma' region, if present.
        [[nodiscard]] const std::shared_ptr<CSectionOutputAtLmaRegion>& AtLmaRegionFunction() const
        {
            return this->m_atLmaRegionFunction;
        }

        /// @brief Reports back the program headers defined, if present.
        [[nodiscard]] const SharedPtrVector<CSectionOutputPhdr>& ProgramHeaders() const
        {
            return this->m_programHeaders;
        }

        /// @brief Reports back the 'Fill' expression, if preent.
        [[nodiscard]] const std::shared_ptr<CSectionOutputFillExpression>& FillExpression() const
        {
            return this->m_fillExpression;
        }

        /// @brief Reports back the outer content, which includes all parsed content.
        [[nodiscard]] const SharedPtrVector<CParsedContentBase>& InnerContent() const
        {
            return this->m_innerContent;
        }

        /// @copydoc CParsedContentBase::AggregateViolation
        [[nodiscard]] const SharedPtrVector<CViolationBase> AggregateViolation() const override;

        /// @brief Produces debug information on what this object represents.
        [[nodiscard]] const std::string ToDebugInfo(uint32_t depth, const CLinkerScriptFile& linkerScriptFile) const override;
    };
}

#endif
