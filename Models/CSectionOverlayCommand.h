#ifndef CSECTION_OVERLAY_H__
#define CSECTION_OVERLAY_H__

#include <vector>
#include <memory>
#include "CLinkerScriptContentBase.h"

namespace VisualLinkerScript::Models
{
    /// @brief Represents a single 'Output' segment in the "SECTIONS"
    class CSectionOverlayCommand : public CLinkerScriptContentBase
    {   
    private:
        CRawEntry m_headerEntry;        
        CRawEntry m_colonEntry;
        CRawEntry m_noCrossRefsEntry;        
        CRawEntry m_bracketOpenEntry;
        CRawEntry m_bracketCloseEntry;                
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_preColonContent;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_postColonContent;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_endingContent;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_innerContent;

    public:
        /// @brief Default constructor, accessible to inheritors only        
        explicit CSectionOverlayCommand(CRawEntry headerEntry,                                        
                                        CRawEntry colonEntry,
                                        CRawEntry noCrossRefsEntry,                                        
                                        CRawEntry bracketOpenEntry,
                                        CRawEntry bracketCloseEntry,                                                                                
                                        std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& preColonContent,
                                        std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& postColonContent,
                                        std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& innerContent,
                                        std::vector<std::shared_ptr<CLinkerScriptContentBase>>&& endingContent,
                                        std::vector<CRawEntry>&& rawElements,
                                        SharedPtrVector<CViolationBase>&& violations)
            : CLinkerScriptContentBase(std::move(rawElements), std::move(violations)),
              m_headerEntry(headerEntry),              
              m_colonEntry(colonEntry),
              m_noCrossRefsEntry(noCrossRefsEntry),              
              m_bracketOpenEntry(bracketOpenEntry),
              m_bracketCloseEntry(bracketCloseEntry),
              m_preColonContent(std::move(preColonContent)),
              m_postColonContent(std::move(postColonContent)),
              m_endingContent(std::move(endingContent)),
              m_innerContent(std::move(innerContent))
        {}        

    public:

        /// @brief [Non-Optional] Header entry (i.e. 'OVERLAY').
        const CRawEntry HeaderEntry()
        {
            return this->m_headerEntry;
        }

        /// @brief [Non-Optional] Reports back the 'Colon' entry.
        const CRawEntry ColonEntry()
        {
            return this->m_colonEntry;
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

        /// @brief [Non-Optional] Reports the content defined inside the 'OVERLAY'
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& InnerContent()
        {
            return this->m_innerContent;
        }

        /// @brief [Non-Optional] Reports the content defined inside between the header and colon
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& PreColonContent()
        {
            return this->m_preColonContent;
        }

        /// @brief [Non-Optional] Reports the content defined inside after the colon and before the bracket-open
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& PostColonContent()
        {
            return this->m_postColonContent;
        }

        /// @brief [Non-Optional] Reports the content defined inside after bracket-closure
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>>& EndingContent()
        {
            return this->m_endingContent;
        }

        /// @brief Reports back the type of this object.        
        ContentType Type() override
        {
            return ContentType::SectionsOverlayStatement;
        }

        /// @brief Produces debug information on what this object represents.
        const virtual std::string ToDebugInfo(uint32_t depth) override;
    };
}

#endif
