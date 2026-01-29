#ifndef STYLER_REMOVE_LINE_H_
#define STYLER_REMOVE_LINE_H_

#include <cstdint>
#include "Components/QLinkerScriptSession/EditorAction/SEditorActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction
{
	/// @brief Remove content action.
	struct SEditorRemoveLine : SEditorActionBase
	{
		/// @copydoc SEditorActionBase::ActionType()
		EEditorActionType ActionType() override
		{
			return EEditorActionType::RemoveLine;
		}

		/// @brief Default constructor;
		SEditorRemoveLine(const uint32_t lineNumber, const uint32_t columnIndex) :
			SEditorActionBase(lineNumber, columnIndex)
		{}
	};
}

#endif
