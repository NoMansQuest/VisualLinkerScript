#ifndef AUTO_STYLER_H_
#define AUTO_STYLER_H_

#include <memory>
#include <vector>
#include "Helpers.h"
#include "Components/QChromeTab/QChromeTabButton.h"
#include "Components/QLinkerScriptSession/EditorAction/SEditorActionBase.h"

namespace VisualLinkerScript::Models
{
	class CLinkerScriptFile;
}

namespace VisualLinkerScript::Components::LinkerScriptSession::AutoStyling
{
	class CAutoStyler
	{
	public:
		/// @brief When text is expected to be inserted (either through key-press or 'Paste')
		static LinqVector<EditorAction::SEditorActionBase> ProcessKeyboardEvent(
			const std::shared_ptr<Models::CLinkerScriptFile>& linkerScriptFile,
			const QKeyEvent& keyEvent,
			uint32_t absolutePosition,
			uint32_t lineNumber, 
			uint32_t columnIndex);				
	};	
}

#endif
