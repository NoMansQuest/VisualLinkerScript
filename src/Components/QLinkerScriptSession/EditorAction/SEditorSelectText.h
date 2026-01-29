#ifndef SEDITOR_SELECT_TEXT_H_
#define SEDITOR_SELECT_TEXT_H_

#include <cstdint>
#include "Components/QLinkerScriptSession/EditorAction/SEditorActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction
{
	/// @brief Remove content action.
	struct SEditorSelectText : SEditorActionBase
	{
		/// @copydoc SEditorActionBase::ActionType()
		EEditorActionType ActionType() override
		{
			return EEditorActionType::SelectText;
		}

		/// @brief End line number to remove (immutable).
		const uint32_t EndLineNumber;

		/// @brief End column index to remove (immutable).
		const uint32_t EndColumnIndex;

		/// @brief Default constructor;
		SEditorSelectText(
				const uint32_t lineNumber,
				const uint32_t columnIndex,
				const uint32_t endLineNumber,
				const uint32_t endColumnIndex) :
			SEditorActionBase(lineNumber, columnIndex),
			EndLineNumber(endLineNumber),
			EndColumnIndex(endColumnIndex)
		{}
	};
}

#endif
