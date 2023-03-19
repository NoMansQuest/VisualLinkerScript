#ifndef CSECTION_OVERLAY_H__
#define CSECTION_OVERLAY_H__

#include <vector>
#include <memory>
#include "CLinkerScriptContentBase.h"
#include "../Models/CExpression.h"
#include "../Models/CFunctionCall.h"
#include "../Models/CSectionOutputFillExpression.h"
#include "../Models/CSectionOutputPhdr.h"

namespace VisualLinkerScript::ParsingEngine::Models
{
    /// @brief Represents a single 'Output' segment in the "SECTIONS"
    class CSectionOverlayCommand : public CLinkerScriptContentBase
    {   
    private:
        CRawEntry m_headerEntry;
        std::shared_ptr<CExpression> m_startAddressExpression;
        CRawEntry m_colonEntry;
        CRawEntry m_noCrossRefsEntry;
        std::shared_ptr<CFunctionCall> m_atAddressFunction;
        CRawEntry m_bracketOpenEntry;
        CRawEntry m_bracketCloseEntry;
        std::vector<std::shared_ptr<CSectionOutputPhdr>> m_programHeaders;
        std::shared_ptr<CSectionOutputFillExpression> m_fillExpression;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_parsedContent;

    public:
        /// @brief Default constructor, accessible to inheritors only        
        explicit CSectionOverlayCommand(CRawEntry headerEntry,
                                        std::shared_ptr<CExpression> startAddressExpression,
                                        CRawEntry colonEntry,
                                        CRawEntry noCrossRefsEntry,
                                        std::shared_ptr<CFunctionCall> atAddressFunction,
                                        CRawEntry bracketOpenEntry,
                                        CRawEntry bracketCloseEntry,
                                        std::vector<std::shared_ptr<CSectionOutputPhdr>>&& programHeaders,
                                        std::shared_ptr<CSectionOutputFillExpression> fillExpression,
                                        std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& parsedContent,
                                        std::vector<CRawEntry>&& rawElements,
                                        std::vector<CViolation>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_headerEntry(headerEntry),
              m_startAddressExpression(startAddressExpression),
              m_colonEntry(colonEntry),
              m_noCrossRefsEntry(noCrossRefsEntry),
              m_atAddressFunction(atAddressFunction),
              m_bracketOpenEntry(bracketOpenEntry),
              m_bracketCloseEntry(bracketCloseEntry),
              m_programHeaders(std::move(programHeaders)),
              m_fillExpression(fillExpression),
              m_parsedContent(std::move(parsedContent))
        {}        

    public:

        /// @brief [Non-Optional] Header entry (i.e. 'OVERLAY').
        const CRawEntry HeaderEntry()
        {
            return this->m_headerEntry;
        }

        /// @brief [Optional] Reports the 'Start' expression declared in the header right after header.
        const std::shared_ptr<CExpression> StartAddressExpression()
        {
            return this->m_startAddressExpression;
        }

        /// @brief [Non-Optional] Reports back the 'Colon' entry.
        const CRawEntry ColonEntry()
        {
            return this->m_colonEntry;
        }

        /// @brief [Optional] Reports the 'NOCROSSREFS' entry (if present).
        const CRawEntry NoCrossRefsEntry()
        {
            return this->m_noCrossRefsEntry;
        }

        /// @brief [Optional] Reports the 'NOCROSSREFS' entry (if present).
        const std::shared_ptr<CFunctionCall> AtAddressFunction()
        {
            return this->m_atAddressFunction;
        }

        /// @brief [Non-Optional] Reports the curly-bracket open entry.
        CRawEntry BracketOpenEntry()
        {
            return this->m_bracketOpenEntry;
        }

        /// @brief [Non-Optional] Reports the curly-bracket close entry.
        CRawEntry BracketCloseEntry()
        {
            return this->m_bracketCloseEntry;
        }

        /// @brief [Optional] Reports the ':phdr' entries at the end of the bracket.
        const std::vector<std::shared_ptr<CSectionOutputPhdr>>& ProgramHeaders()
        {
            return this->m_programHeaders;
        }

        /// @brief [Optional] Reports the '=fill' entries at the end of the bracket.
        const std::shared_ptr<CSectionOutputFillExpression> FillExpression()
        {
            return this->m_fillExpression;
        }

        /// @brief [Non-Optional] Reports the content defined inside the 'OVERLAY'
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& ParsedContent()
        {
            return this->m_parsedContent;
        }

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SectionsOverlayStatement;
        }
    };
}

#endif
