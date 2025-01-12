#ifndef CDRCVIOLATION_H__
#define CDRCVIOLATION_H__

#include <string>
#include <vector>
#include <memory>
#include <Models/CParsedContentBase.h>
#include "EDrcViolationCode.h"
#include "../Models/CViolationBase.h"
#include "../Helpers.h"

namespace VisualLinkerScript::DrcEngine 
{
	class CCorrectiveAction;
	using namespace VisualLinkerScript;
    using namespace VisualLinkerScript::Models;

    /// @brief This object contains information about a detected violation in the Linker Script
    class CDrcViolation : public CViolationBase
    {
    private:
        EDrcViolationCode m_violationCode;
        LinqVector<CParsedContentBase> m_involvedElements;
        LinqVector<CDrcViolation> m_subitems;
        std::string m_violationMessage;
        std::string m_title;
        std::string m_contentSensitivePath;        
        std::shared_ptr<CCorrectiveAction> m_correctiveAction;

    public:
        /// @brief Default constructor, reporting the @see {involvedEntries} only
        explicit CDrcViolation(LinqVector<CParsedContentBase> involvedElements,
                               std::string title,
                               std::string violationMessage,
                               std::string contentSensitivePath,
                               LinqVector<CDrcViolation> subitems,
                               const std::shared_ptr<CCorrectiveAction>& correctiveAction,
                               const EDrcViolationCode violationCode,
                               const ESeverityCode drcViolationSeverity = ESeverityCode::Error)
            : CViolationBase(drcViolationSeverity),
    	      m_violationCode(violationCode),
              m_involvedElements(std::move(involvedElements)),
              m_subitems(std::move(subitems)),
              m_violationMessage(std::move(violationMessage)),
              m_title(std::move(title)),
              m_contentSensitivePath(std::move(contentSensitivePath)),
              m_correctiveAction(correctiveAction)
        {}

        /// @brief Specialized constructor - ContentSensitivePath not taken
        explicit CDrcViolation(LinqVector<CParsedContentBase> involvedElements,
                               std::string title,
                               std::string violationMessage,
                               LinqVector<CDrcViolation>&& subitems,
                               const std::shared_ptr<CCorrectiveAction>& correctiveAction,
                               const EDrcViolationCode violationCode,
							   const ESeverityCode drcViolationSeverity = ESeverityCode::Error)
            : CViolationBase(drcViolationSeverity),
    		  m_violationCode(violationCode),              
              m_involvedElements(std::move(involvedElements)),
              m_subitems(subitems),
              m_violationMessage(std::move(violationMessage)),
              m_title(std::move(title)),
              m_correctiveAction(correctiveAction)
        {}

        /// @brief Specialized constructor - ContentSensitivePath and SubItems not taken
        explicit CDrcViolation(LinqVector<CParsedContentBase> involvedElements,
                               std::string title,
                               std::string violationMessage,
                               const std::shared_ptr<CCorrectiveAction>& correctiveAction,
                               const EDrcViolationCode violationCode,
							   const ESeverityCode drcViolationSeverity = ESeverityCode::Error)
            : CViolationBase(drcViolationSeverity),
    	      m_violationCode(violationCode),              
              m_involvedElements(std::move(involvedElements)),              
              m_violationMessage(std::move(violationMessage)),
              m_title(std::move(title)),
              m_correctiveAction(correctiveAction)
        {}

        /// @brief Specialized constructor - ContentSensitivePath, SubItems and CorrectiveAction not taken
        explicit CDrcViolation(LinqVector<CParsedContentBase> involvedElements,
                               std::string title,
                               std::string violationMessage,
                               const EDrcViolationCode violationCode,
							   const ESeverityCode drcViolationSeverity = ESeverityCode::Error)
            : CViolationBase(drcViolationSeverity),
    	      m_violationCode(violationCode),
              m_involvedElements(std::move(involvedElements)),
              m_violationMessage(std::move(violationMessage)),
              m_title(std::move(title))
        {}

        /// @brief Specialized constructor - ContentSensitivePath, SubItems and CorrectiveAction not taken
        explicit CDrcViolation(std::shared_ptr<CParsedContentBase> involvedElement,
                               std::string title,
                               std::string violationMessage,
                               const EDrcViolationCode violationCode,
							   const ESeverityCode drcViolationSeverity = ESeverityCode::Error)    
            : CViolationBase(drcViolationSeverity),
    		  m_violationCode(violationCode),            
              m_involvedElements({ involvedElement }),
              m_violationMessage(std::move(violationMessage)),
              m_title(std::move(title))
        {}

    public:
        /// @brief Reports back a list of involved elements
        [[nodiscard]] LinqVector<CParsedContentBase> InvolvedElements() const
        {
            return this->m_involvedElements;
        }

        /// @brief Reports back a human-friendly violation message
        const std::string& ViolationMessage() {
            return this->m_violationMessage;
        }

        /// @brief Reports back the title of the violation. This can be used for visualization purposes.
        [[nodiscard]] const std::string& Title() const{
            return this->m_title;
        }

        /// @brief Reports back the content-sensitive path of the violation. This can be used for tracking purposes.
        [[nodiscard]] const std::string& ContentSensitivePath() const {
            return this->m_contentSensitivePath;
        }

        /// @brief Reports back the corrective action that helps resolve this violation.
        /// @remarks If no corrective action is available, 'nullptr' will be returned.
        std::shared_ptr<CCorrectiveAction> CorrectiveAction(){
            return this->m_correctiveAction;
        }

        /// @brief Reports back the tpe of violation        
        [[nodiscard]] EDrcViolationCode Code() const {
            return this->m_violationCode;
        }

        /// @brief Reports back sub items (if any) belonging to this violation
        [[nodiscard]] LinqVector<CDrcViolation> Subitems() const{
            return this->m_subitems;
        }

        /// @brief Type of violation.
        EViolationType Type() override {
            return EViolationType::DrcViolation;
        }
    };
}

#endif // CPARSERVIOLATION_H__
