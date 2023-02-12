#ifndef CSECTION_OUTPUT_STATEMENT_H__
#define CSECTION_OUTPUT_STATEMENT_H__

#include <vector>
#include <memory>
#include <string>
#include "../Models/CFunctionCall.h"
#include "../Models/CSectionOutputToRegion.h"
#include "../Models/CSectionOutputAtLmaRegion.h"
#include "../Models/CSectionOutputPhdr.h"
#include "../Models/CSectionOutputFillExpression.h"
#include "../Models/CSectionOutputType.h"
#include "../Models/Raw/CRawEntry.h"
#include "../Models/CViolation.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a single 'Output' segment in the "SECTIONS"
    class CSectionOutputStatement : public CLinkerScriptContentBase
    {   
    private:
        CRawEntry m_sectionOutputNameEntry;
        std::shared_ptr<CExpression> m_addressExpression;
        std::shared_ptr<CSectionOutputType> m_sectionOutputType;
        std::shared_ptr<CFunctionCall> m_atLmaFunction;
        std::shared_ptr<CFunctionCall> m_alignFunction;
        CRawEntry m_alignWithInputEntry;
        std::shared_ptr<CFunctionCall> m_subAlignFunctionCall;
        CRawEntry m_colonEntry;
        CRawEntry m_openingBracketEntry;
        CRawEntry m_closingBracketEntry;
        std::shared_ptr<CSectionOutputToRegion> m_toVmaRegion;
        std::shared_ptr<CSectionOutputAtLmaRegion> m_atLmaRegionFunction;
        std::vector<CSectionOutputPhdr> m_programHeaders;
        std::shared_ptr<CSectionOutputFillExpression> m_fillExpression;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_parsedContent;

    public:
        /// @brief Default constructor, accessible to inheritors only
        /// @param rawElements A list of object this element is comprised of.
        explicit CSectionOutputStatement(CRawEntry sectionOutputNameEntry,
                                         std::shared_ptr<CExpression> addressExpression,
                                         std::shared_ptr<CSectionOutputType> sectionOutputType,
                                         std::shared_ptr<CFunctionCall> atLmaFunction,
                                         std::shared_ptr<CFunctionCall> alignFunction,
                                         CRawEntry alignWithInputEntry,
                                         std::shared_ptr<CFunctionCall> subAlignFunctionCall,
                                         CRawEntry colonEntry,
                                         CRawEntry openingBracketEntry,
                                         CRawEntry closingBracketEntry,
                                         std::shared_ptr<CSectionOutputToRegion> toVmaRegion,
                                         std::shared_ptr<CSectionOutputAtLmaRegion> atLmaRegionFunction,
                                         std::vector<CSectionOutputPhdr>&& programHeaders,
                                         std::shared_ptr<CSectionOutputFillExpression> fillExpression,
                                         std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& parsedContent,
                                         std::vector<CRawEntry>&& rawElements,
                                         std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_sectionOutputNameEntry(sectionOutputNameEntry),
              m_addressExpression(addressExpression),
              m_sectionOutputType(sectionOutputType),
              m_atLmaFunction(atLmaFunction),
              m_alignFunction(alignFunction),
              m_alignWithInputEntry(alignWithInputEntry),
              m_subAlignFunctionCall(subAlignFunctionCall),
              m_colonEntry(colonEntry),
              m_openingBracketEntry(openingBracketEntry),
              m_closingBracketEntry(closingBracketEntry),
              m_toVmaRegion(toVmaRegion),
              m_atLmaRegionFunction(atLmaRegionFunction),
              m_programHeaders(std::move(programHeaders)),
              m_fillExpression(fillExpression),
              m_parsedContent(std::move(parsedContent))
        {}        

    public:
        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SectionsOutputStatement;
        }

        /// @brief Reports back the name of the SectionOutput
        CRawEntry SectionOutputNameEntry()
        {
            return this->m_sectionOutputNameEntry;
        }

        /// @brief Reports back 'Address' expression should it be present
        std::shared_ptr<CExpression> AddressExpression()
        {
            return this->m_addressExpression;
        }

        /// @brief Reports back the type of this section-output
        std::shared_ptr<CSectionOutputType> SectionOutputType()
        {
            return this->m_sectionOutputType;
        }

        /// @brief Reports back 'AtLma' function call
        std::shared_ptr<CFunctionCall> AtLmaFunction()
        {
            return this->m_atLmaFunction;
        }

        /// @brief Reports back he 'Align' function call, should it be present
        std::shared_ptr<CFunctionCall> AlignFunction()
        {
            return this->m_alignFunction;
        }

        /// @brief Reports back 'AlignWithInput' defintion, should it be present.
        CRawEntry AlignWithInputEntry()
        {
            return this->m_alignWithInputEntry;
        }

        /// @brief Reports back the 'SubAlign' function call, if present.
        std::shared_ptr<CFunctionCall> SubAlignFunctionCall()
        {
            return this->m_subAlignFunctionCall;
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
        std::shared_ptr<CSectionOutputToRegion> ToVmaRegion()
        {
            return this->m_toVmaRegion;
        }

        /// @brief Reports back the 'AtLma' region, if present.
        std::shared_ptr<CSectionOutputAtLmaRegion> AtLmaRegionFunction()
        {
            return this->m_atLmaRegionFunction;
        }

        /// @brief Reports back the program headers defined, if present.
        std::vector<CSectionOutputPhdr> ProgramHeaders()
        {
            return this->m_programHeaders;
        }

        /// @brief Reports back the 'Fill' expression, if preent.
        std::shared_ptr<CSectionOutputFillExpression> FillExpression()
        {
            return this->m_fillExpression;
        }

        /// @brief Reports back the outer content, which includes all parsed content.
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& ParsedContent()
        {
            return this->m_parsedContent;
        }
    };
}

#endif
