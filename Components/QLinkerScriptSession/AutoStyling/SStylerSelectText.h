#ifndef STYLER_REMOVE_CONTENT_H
#define STYLER_REMOVE_CONTENT_H

#include <cstdint>
#include "SStylerActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::AutoStyling
{
	/// @brief Remove content action.
	struct SStylerSelectText : SStylerActionBase
	{
		/// @brief End line number to remove (immutable).
		const uint32_t EndLineNumber;

		/// @brief End column index to remove (immutable).
		const uint32_t EndColumnIndex;

		/// @brief Default constructor;
		SStylerSelectText(
			const uint32_t lineNumber,
			const uint32_t columnIndex,
			const uint32_t endLineNumber,
			const uint32_t endColumnIndex) :
			SStylerActionBase(lineNumber, columnIndex),
			EndLineNumber(endLineNumber),
			EndColumnIndex(endColumnIndex)
		{}
	};
}

#endif
