#ifndef STYLER_REPLACE_LINE_H_
#define STYLER_REPLACE_LINE_H_

#include <cstdint>
#include <string>
#include "SEditorActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction
{
	/// @brief Add text action.
	struct SEditorReplaceContent : SEditorActionBase
	{
		/// @copydoc SEditorActionBase::ActionType()
		EEditorActionType ActionType() override
		{
			return EEditorActionType::ReplaceContent;
		}

		/// @brief Line number the action takes place (immutable).
		const std::string TextToReplace;

		/// @brief End Line number of the section to be replaced
		const uint32_t EndLineNumber;

		/// @brief End column index of the section to be replaced
		const uint32_t EndColumnIndex;

		/// @brief Default constructor;
		///	@param lineNumber Line to replace
		///	@param columnIndex Ignored for this action
		///	@param endLineNumber Line to replace
		///	@param endColumnIndex Ignored for this action
		///	@param textToReplace Text to set for the line
		SEditorReplaceContent(
				const uint32_t lineNumber, 
				const uint32_t columnIndex,
				const uint32_t endLineNumber,
				const uint32_t endColumnIndex,
				std::string textToReplace) :
			SEditorActionBase(lineNumber, columnIndex),
			TextToReplace(std::move(textToReplace)),
			EndLineNumber(endLineNumber),
			EndColumnIndex(endColumnIndex)
		{}
	};
}

#endif
