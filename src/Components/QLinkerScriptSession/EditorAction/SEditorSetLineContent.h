#ifndef SET_LINE_CONTENT_H__
#define SET_LINE_CONTENT_H__

#include <cstdint>
#include <string>
#include "Components/QLinkerScriptSession/EditorAction/SEditorActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction
{
	/// @brief Add text action.
	struct SEditorSetLineContent : SEditorActionBase
	{
		/// @copydoc SEditorActionBase::ActionType()
		EEditorActionType ActionType() override
		{
			return EEditorActionType::SetLineContent;
		}

		/// @brief Text to set at the given line number
		const std::string TextToSet;

		/// @brief Default constructor;
		///	@param lineNumber Line number to set text for.
		///	@param columnIndex (Reserved)
		///	@param textToSet Text to set for the given line
		SEditorSetLineContent(
			const uint32_t lineNumber,
			const uint32_t columnIndex,
			std::string textToSet) :
			SEditorActionBase(lineNumber, columnIndex),
			TextToSet(std::move(textToSet))
		{}
	};
}

#endif
