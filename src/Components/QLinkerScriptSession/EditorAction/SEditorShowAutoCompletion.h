#ifndef SEDITOR_SHOW_AUTOCOMPLETION_H__
#define SEDITOR_SHOW_AUTOCOMPLETION_H__

#include <cstdint>
#include <string>
#include <vector>

#include "Components/QLinkerScriptSession/EditorAction/SEditorActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction
{
	/// @brief Type of AutoCompletion suggestion action
	enum class AutoCompletionSuggestionType
	{
		Variable,
		Function,
		Method,
		Directive,
	};

	/// @brief Auto-completion value object.
	struct AutoCompleteSuggestion
	{
		std::string Value;
		AutoCompletionSuggestionType Type;
	};

	/// @brief Remove content action.
	struct SEditorShowAutoCompletion : SEditorActionBase
	{
	private:
		std::vector<std::string> m_suggestions;

	public:

		/// @copydoc SEditorActionBase::ActionType()
		EEditorActionType ActionType() override
		{
			return EEditorActionType::ShowAutoCompletion;
		}

		/// @brief Returns suggestions of this action.
		const std::vector<std::string>& Suggestions ()
		{
			return this->m_suggestions;
		}

		/// @brief Default constructor;
		SEditorShowAutoCompletion(
			const uint32_t lineNumber,
			const uint32_t columnIndex,
			const std::vector<std::string>& suggestions) :
			SEditorActionBase(lineNumber, columnIndex),
			m_suggestions(suggestions)
		{}
	};
}

#endif
