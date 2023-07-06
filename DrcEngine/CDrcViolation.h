#ifndef CDRCVIOLATION_H__
#define CDRCVIOLATION_H__

#include <string>
#include <vector>
#include <memory>
#include <Models/CLinkerScriptContentBase.h>
#include <Models/Intervention/CIntervention.h>
#include "EDrcViolationCode.h"
#include "EDrcViolationSeverity.h"
#include "../Models/CViolationBase.h"
#include "../Helpers.h"

namespace VisualLinkerScript::DrcEngine 
{
    using namespace VisualLinkerScript;
    using namespace VisualLinkerScript::Models;
    using namespace VisualLinkerScript::Models::Intervention;

    /// @brief This object contains information about a detected violation in the Linker Script
    class CDrcViolation : public CViolationBase
    {
    private:
        EDrcViolationCode m_violationCode;
        EDrcViolationSeverity m_violationSeverity;
        SharedPtrVector<CLinkerScriptContentBase> m_involvedElements;
        SharedPtrVector<CDrcViolation> m_subitems;
        std::string m_violationMessage;
        std::string m_title;
        std::string m_contentSensitivePath;        
        std::shared_ptr<CIntervention> m_correctiveAction;

    public:
        /// @brief Default constructor, reporting the @see {involvedEntries} only
        explicit CDrcViolation(SharedPtrVector<CLinkerScriptContentBase> involvedElements,
                               std::string title,
                               std::string violationMessage,
                               std::string contentSensitivePath,
                               SharedPtrVector<CDrcViolation>&& subitems,
                               std::shared_ptr<CIntervention> correctiveAction,                                
                               EDrcViolationCode violationCode,
                               EDrcViolationSeverity drcViolationSeverity)
            : m_violationCode(violationCode),
              m_violationSeverity(drcViolationSeverity),
              m_involvedElements(std::move(involvedElements)),
              m_subitems(subitems),
              m_violationMessage(violationMessage),
              m_title(title),
              m_contentSensitivePath(contentSensitivePath),
              m_correctiveAction(correctiveAction)
        {}

    public:
        /// @brief Reports back a list of involved elements
        const SharedPtrVector<CLinkerScriptContentBase> InvolvedElements(){
            return this->m_involvedElements;
        }

        /// @brief Reports back a human-friendly violation message
        const std::string& ViolationMessage() {
            return this->m_violationMessage;
        }

        /// @brief Reports back the title of the violation. This can be used for visualization purposes.
        const std::string& Title() {
            return this->m_title;
        }

        /// @brief Reports back the content-sensitive path of the violation. This can be used for tracking purposes.
        const std::string& ContentSensitivePath() {
            return this->m_contentSensitivePath;
        }

        /// @brief Reports back the corrective action that helps resolve this violation.
        /// @remarks If no corrective action is available, 'nullptr' will be returned.
        const std::shared_ptr<CIntervention> CorrectiveAction(){
            return this->m_correctiveAction;
        }

        /// @brief Reports back the tpe of violation        
        const EDrcViolationCode Code() {
            return m_violationCode;
        }

        /// @brief Reports back the severity of the violation
        const EDrcViolationSeverity Severity() {
            return m_violationSeverity;
        }

        /// @brief Reports back sub items (if any) belonging to this violation
        const SharedPtrVector<CDrcViolation> Subitems(){
            return this->m_subitems;
        }
    };
}

#endif // CPARSERVIOLATION_H__
