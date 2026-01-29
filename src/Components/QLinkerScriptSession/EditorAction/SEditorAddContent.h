#ifndef ADD_CONTENT_H__
#define ADD_CONTENT_H__

#include <cstdint>
#include <string>
#include "Components/QLinkerScriptSession/EditorAction/SEditorActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction
{
	/// @brief Add text action.
	struct SEditorAddContent : SEditorActionBase
	{
		/// @copydoc SEditorActionBase::ActionType()
		EEditorActionType ActionType() override
		{
			return EEditorActionType::AddContent;
		}

		/// @brief Line number the action takes place (immutable).
		const std::string TextToInsert;		

		/// @brief Default constructor;
		SEditorAddContent(
			const uint32_t lineNumber,
			const uint32_t columnIndex,
			std::string textToInsert) :
			SEditorActionBase(lineNumber, columnIndex),
			TextToInsert(std::move(textToInsert))
		{}
	};
}

#endif
