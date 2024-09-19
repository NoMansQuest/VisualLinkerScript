#ifndef AUTO_STYLER_H_
#define AUTO_STYLER_H_

#include <memory>
#include <vector>
#include "Helpers.h"

struct SStylerActionBase;
namespace VisualLinkerScript::Models::Raw
{
	class CRawFile;
}

namespace VisualLinkerScript::Components::LinkerScriptSession::AutoStyling
{
	class CAutoStyler
	{
		/// @brief When text is expected to be inserted (either through key-press or 'Paste')
		SharedPtrVector<SStylerActionBase> TextInsertionRequested(
			std::shared_ptr<Models::Raw::CRawFile> lexedScript,
			std::string text, 
			uint32_t lineNumber, 
			uint32_t columnIndex);

		/// @brief When text is expected to be removed (either through pressing 'Delete' or 'Cut' action)
		SharedPtrVector<SStylerActionBase> TextRemovalRequested(
			std::shared_ptr<Models::Raw::CRawFile> lexedScript,			
			uint32_t startLineNumber,
			uint32_t startColumnIndex,
			uint32_t endLineNumber,
			uint32_t endColumnIndex);
				
	};	
}

#endif
