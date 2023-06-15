#ifndef CDRCVIOLATION_H__
#define CDRCVIOLATION_H__

#include <string>
#include <vector>
#include <memory>
#include <Models/CLinkerScriptContentBase.h>
#include <Models/Intervention/CIntervention.h>
#include "EDrcViolationCode.h"
#include "EDrcViolationSeverity.h"

using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::Models::Intervention;

namespace VisualLinkerScript::DrcEngine 
{
    /// @brief This object contains information about a detected violation in the Linker Script
    class CDrcViolation
    {
    private:
        EDrcViolationCode m_violationCode;
        EDrcViolationSeverity m_violationSeverity;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_involvedElements;
        std::string m_violationMessage;
        std::string m_title;
        std::string m_contentSensitivePath;        
        std::shared_ptr<CIntervention> m_correctiveAction;

    public:
        /// @brief Default constructor, reporting the @see {involvedEntries} only
        explicit CDrcViolation(std::vector<std::shared_ptr<CLinkerScriptContentBase>> involvedElements,
                               std::string&& title,
                               std::string&& violationMessage,
                               std::string&& contentSensitivePath,
                               std::shared_ptr<CIntervention> correctiveAction,
                               EDrcViolationCode violationCode,
                               EDrcViolationSeverity drcViolationSeverity)
            : m_violationCode(violationCode),
              m_violationSeverity(drcViolationSeverity),
              m_involvedElements(std::move(involvedElements)),
              m_violationMessage(std::move(violationMessage)),
              m_title(std::move(title)),
              m_contentSensitivePath(std::move(contentSensitivePath))
        {}

    public:
        /// @brief Reports back a list of involved elements
        const std::vector<std::shared_ptr<CLinkerScriptContentBase>> InvolvedElements(){
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
    };
}

#endif // CPARSERVIOLATION_H__
