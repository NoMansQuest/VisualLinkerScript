#ifndef EEDITOR_ACTION_TYPE_H__
#define EEDITOR_ACTION_TYPE_H__

#include <cstdint>

namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction
{
	/// @brief Various types of editor action.
	enum class EEditorActionType : uint32_t
	{
		AddContent,
		AddEmptyLine,
		SetLineContent,
		RemoveContent,
		RemoveLine,
		ReplaceContent,
		SelectText,
		ShowAutoCompletion,
		SetCaretPosition,
	};
}

#endif