#ifndef ADD_EMPTY_LINE_H__
#define ADD_EMPTY_LINE_H__

#include <cstdint>
#include "Components/QLinkerScriptSession/EditorAction/SEditorActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::EditorAction
{
	/// @brief Add text action.
	struct SEditorAddEmptyLine : SEditorActionBase
	{
		/// @copydoc SEditorActionBase::ActionType()
		EEditorActionType ActionType() override
		{
			return EEditorActionType::AddEmptyLine;
		}

		/// @brief Default constructor;
		///	@param lineNumber The line under which the new line will be added
		///	@param columnIndex (Reserved)
		SEditorAddEmptyLine(
			const uint32_t lineNumber,
			const uint32_t columnIndex) :
			SEditorActionBase(lineNumber, columnIndex)
		{}
	};
}

#endif
