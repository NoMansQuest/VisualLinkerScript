#ifndef STYLER_REMOVE_LINE_H_
#define STYLER_REMOVE_LINE_H_

#include <cstdint>
#include "SStylerActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::AutoStyling
{
	/// @brief Remove content action.
	struct SStylerRemoveLine : SStylerActionBase
	{
		/// @brief Default constructor;
		SStylerRemoveLine(const uint32_t lineNumber, const uint32_t columnIndex) :
			SStylerActionBase(lineNumber, columnIndex)
		{}
	};
}

#endif
