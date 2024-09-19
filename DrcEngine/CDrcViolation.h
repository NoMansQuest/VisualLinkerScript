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
                               SharedPtrVector<CDrcViolation> subitems,
                               const std::shared_ptr<CIntervention>& correctiveAction,                                
                               const EDrcViolationCode violationCode,
                               const EDrcViolationSeverity drcViolationSeverity)
            : m_violationCode(violationCode),
              m_violationSeverity(drcViolationSeverity),
              m_involvedElements(std::move(involvedElements)),
              m_subitems(std::move(subitems)),
              m_violationMessage(std::move(violationMessage)),
              m_title(std::move(title)),
              m_contentSensitivePath(std::move(contentSensitivePath)),
              m_correctiveAction(correctiveAction)
        {}

        /// @brief Specialized constructor - ContentSensitivePath not taken
        explicit CDrcViolation(SharedPtrVector<CLinkerScriptContentBase> involvedElements,
                               std::string title,
                               std::string violationMessage,
                               SharedPtrVector<CDrcViolation>&& subitems,
                               const std::shared_ptr<CIntervention>& correctiveAction,
                               const EDrcViolationCode violationCode,
                               const EDrcViolationSeverity drcViolationSeverity)
            : m_violationCode(violationCode),
              m_violationSeverity(drcViolationSeverity),
              m_involvedElements(std::move(involvedElements)),
              m_subitems(subitems),
              m_violationMessage(std::move(violationMessage)),
              m_title(std::move(title)),
              m_correctiveAction(correctiveAction)
        {}

        /// @brief Specialized constructor - ContentSensitivePath and SubItems not taken
        explicit CDrcViolation(SharedPtrVector<CLinkerScriptContentBase> involvedElements,
                               std::string title,
                               std::string violationMessage,
                               const std::shared_ptr<CIntervention>& correctiveAction,
                               const EDrcViolationCode violationCode,
                               const EDrcViolationSeverity drcViolationSeverity)
            : m_violationCode(violationCode),
              m_violationSeverity(drcViolationSeverity),
              m_involvedElements(std::move(involvedElements)),              
              m_violationMessage(std::move(violationMessage)),
              m_title(std::move(title)),
              m_correctiveAction(correctiveAction)
        {}

        /// @brief Specialized constructor - ContentSensitivePath, SubItems and CorrectiveAction not taken
        explicit CDrcViolation(SharedPtrVector<CLinkerScriptContentBase> involvedElements,
                               std::string title,
                               std::string violationMessage,
                               const EDrcViolationCode violationCode,
                               const EDrcViolationSeverity drcViolationSeverity)
            : m_violationCode(violationCode),
              m_violationSeverity(drcViolationSeverity),
              m_involvedElements(std::move(involvedElements)),
              m_violationMessage(std::move(violationMessage)),
              m_title(std::move(title))
        {}

        /// @brief Specialized constructor - ContentSensitivePath, SubItems and CorrectiveAction not taken
        explicit CDrcViolation(std::shared_ptr<CLinkerScriptContentBase> involvedElement,
                               std::string title,
                               std::string violationMessage,
                               const EDrcViolationCode violationCode,
					           const EDrcViolationSeverity drcViolationSeverity)
            : m_violationCode(violationCode),
            m_violationSeverity(drcViolationSeverity),
            m_involvedElements({ involvedElement }),
            m_violationMessage(std::move(violationMessage)),
            m_title(std::move(title))
        {}

    public:
        /// @brief Reports back a list of involved elements
        const SharedPtrVector<CLinkerScriptContentBase> InvolvedElements() const {
            return this->m_involvedElements;
        }

        /// @brief Reports back a human-friendly violation message
        const std::string& ViolationMessage() {
            return this->m_violationMessage;
        }

        /// @brief Reports back the title of the violation. This can be used for visualization purposes.
        const std::string& Title() const{
            return this->m_title;
        }

        /// @brief Reports back the content-sensitive path of the violation. This can be used for tracking purposes.
        const std::string& ContentSensitivePath() const {
            return this->m_contentSensitivePath;
        }

        /// @brief Reports back the corrective action that helps resolve this violation.
        /// @remarks If no corrective action is available, 'nullptr' will be returned.
        const std::shared_ptr<CIntervention> CorrectiveAction(){
            return this->m_correctiveAction;
        }

        /// @brief Reports back the tpe of violation        
        const EDrcViolationCode Code() const {
            return this->m_violationCode;
        }

        /// @brief Reports back the severity of the violation
        EDrcViolationSeverity Severity() const {
            return m_violationSeverity;
        }

        /// @brief Reports back sub items (if any) belonging to this violation
        SharedPtrVector<CDrcViolation> Subitems() const{
            return this->m_subitems;
        }

        /// @brief Type of violation.
        virtual EViolationType Type() override {
            return EViolationType::DrcViolation;
        }
    };
}

#endif // CPARSERVIOLATION_H__
