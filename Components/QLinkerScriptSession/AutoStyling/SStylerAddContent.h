#ifndef STYLER_ADD_CONTENT_H_
#define STYLER_ADD_CONTENT_H_

#include <cstdint>
#include <string>
#include "SStylerActionBase.h"

namespace VisualLinkerScript::Components::LinkerScriptSession::AutoStyling
{
	/// @brief Add text action.
	struct SStylerAddContent : SStylerActionBase
	{
		/// @brief Line number the action takes place (immutable).
		const std::string TextToInsert;

		/// @brief Default constructor;
		SStylerAddContent(
			const uint32_t lineNumber,
			const uint32_t columnIndex,
			std::string textToInsert) :
			SStylerActionBase(lineNumber, columnIndex),
			TextToInsert(std::move(textToInsert))
		{}
	};
}

#endif
