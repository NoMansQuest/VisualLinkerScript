#ifndef STYLER_ACTION_BASE_H_
#define STYLER_ACTION_BASE_H_

#include <cstdint>
#include "EEditorActionType.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction
{
	/// @brief Base of all actions proposed by the AutoStyler
	struct SEditorActionBase
	{
	protected:
		~SEditorActionBase() = default;

	public:
		/// @brief Line number the action takes place (immutable).
		const uint32_t LineNumber;

		/// @brief Column the action takes place (immutable).
		const uint32_t ColumnIndex;

		/// @brief Reports the type of the action.
		virtual EEditorActionType ActionType() = 0;

		/// @brief Default constructor;
		SEditorActionBase(uint32_t lineNumber, uint32_t columnIndex) :
			LineNumber(lineNumber), ColumnIndex(columnIndex)
		{}
	};
}

#endif
