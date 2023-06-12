#ifndef CDRCVIOLATION_H__
#define CDRCVIOLATION_H__

#include <string>
#include <vector>
#include <memory>
#include <Models/CLinkerScriptContentBase.h>
#include "EDrcViolationCode.h"
#include "../Models/CViolationBase.h"

using namespace VisualLinkerScript::Models;

namespace VisualLinkerScript::DrcEngine 
{
    /// @brief This object contains information about a detected violation in the Linker Script
    class CDrcViolation
    {
    private:
        EDrcViolationCode m_violationCode;
        std::vector<std::shared_ptr<CLinkerScriptContentBase>> m_involvedElements;
        std::string m_violationMessage;

    public:
        /// @brief Default constructor, reporting the @see {involvedEntries} only
        explicit CDrcViolation(std::vector<std::shared_ptr<CLinkerScriptContentBase>> involvedElements,
                               std::string&& violationMessage,
                               EDrcViolationCode violationCode)
            : m_violationCode(violationCode),
              m_involvedElements(std::move(involvedElements)),
              m_violationMessage(std::move(violationMessage))
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

        /// @brief Reports back the tpe of violation        
        const EDrcViolationCode Code() {
            return m_violationCode;
        }
    };
}

#endif // CPARSERVIOLATION_H__
