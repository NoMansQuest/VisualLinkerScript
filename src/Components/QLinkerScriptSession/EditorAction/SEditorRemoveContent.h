#ifndef STYLER_REMOVE_CONTENT_H
#define STYLER_REMOVE_CONTENT_H

#include <cstdint>
#include "SEditorActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction
{
	/// @brief Remove content action.
	struct SEditorRemoveContent : SEditorActionBase
	{
		/// @copydoc SEditorActionBase::ActionType()
		EEditorActionType ActionType() override
		{
			return EEditorActionType::RemoveContent;
		}

		/// @brief End line number to remove (immutable).
		const uint32_t EndLineNumber;

		/// @brief End column index to remove (immutable).
		const uint32_t EndColumnIndex;

		/// @brief Default constructor;
		SEditorRemoveContent(
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
