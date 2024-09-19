#ifndef STYLER_REPLACE_LINE_H_
#define STYLER_REPLACE_LINE_H_

#include <cstdint>
#include <string>
#include "SStylerActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::AutoStyling
{
	/// @brief Add text action.
	struct SStylerReplaceLine : SStylerActionBase
	{
		/// @brief Line number the action takes place (immutable).
		const std::string TextToReplace;

		/// @brief Default constructor;
		///	@param lineNumber Line to replace
		///	@param columnIndex Ignored for this action
		///	@param textToReplace Text to set for the line
		SStylerReplaceLine(const uint32_t lineNumber, const uint32_t columnIndex, std::string textToReplace) :
			SStylerActionBase(lineNumber, columnIndex),
			TextToReplace(std::move(textToReplace))
		{}
	};
}

#endif
