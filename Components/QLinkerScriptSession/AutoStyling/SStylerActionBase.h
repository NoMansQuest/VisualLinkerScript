#ifndef STYLER_ACTION_BASE_H_
#define STYLER_ACTION_BASE_H_

#include <cstdint>

namespace VisualLinkerScript::Components::LinkerScriptSession::AutoStyling
{
	/// @brief Base of all actions proposed by the AutoStyler
	struct SStylerActionBase
	{
		/// @brief Line number the action takes place (immutable).
		const uint32_t LineNumber;

		/// @brief Column the action takes place (immutable).
		const uint32_t ColumnIndex;

		/// @brief Default constructor;
		SStylerActionBase(uint32_t lineNumber, uint32_t columnIndex) :
			LineNumber(lineNumber), ColumnIndex(columnIndex)
		{}
	};
}

#endif
