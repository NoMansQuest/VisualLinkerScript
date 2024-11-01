#ifndef SEDITOR_SET_CARET_POSITION_H__
#define SEDITOR_SET_CARET_POSITION_H__

#include <cstdint>
#include <string>
#include "SEditorActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction
{
	/// @brief Sets the cursor to a specific position.
	struct SEditorSetCaretPosition : SEditorActionBase
	{
		/// @copydoc SEditorSetCursorPosition::ActionType()
		EEditorActionType ActionType() override
		{
			return EEditorActionType::SetCaretPosition;
		}

		/// @brief Default constructor;
		SEditorSetCaretPosition(
			const uint32_t lineNumber,
			const uint32_t columnIndex) :
			SEditorActionBase(lineNumber, columnIndex)
		{}
	};
}

#endif
