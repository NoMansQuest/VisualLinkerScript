#ifndef CINTERVENTION_H__
#define CINTERVENTION_H__

#include <vector>
#include <cstdint>
#include <memory>
#include <string>
#include "CActionBase.h"

namespace VisualLinkerScript::Models::Intervention
{
    /// @brief Represents a series of actions to undertake to produce desired outcome
    ///        on the model.
    class CIntervention
    {
    private:
        std::vector<std::shared_ptr<CActionBase>> m_actions;
        std::string m_title;

    public:
        /// @brief Default constructor
        CIntervention(std::string&& title,
                      std::vector<std::shared_ptr<CActionBase>>&& actions)
            : m_actions(std::move(actions)),
              m_title(std::move(title))
        {}

        /// @brief Default destructor
        ~CIntervention()
        {}

    public:
        /// @brief Reports back the title of the intervention.
        const std::string& Title() {
            return this->m_title;
        }

        /// @brief Reports back a list of actions to undertake.
        const  std::vector<std::shared_ptr<CActionBase>>& Actions() {
            return this->m_actions;
        }
    };
}


#endif // CINTERVENTION_H__
