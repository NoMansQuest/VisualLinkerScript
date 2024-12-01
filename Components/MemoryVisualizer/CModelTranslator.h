#ifndef CMODEL_TRANSLATOR_H__
#define CMODEL_TRANSLATOR_H__

#include <memory>
#include "../../Models/CLinkerScriptFile.h"

namespace VisualLinkerScript::Components::MemoryVisualizer
{
	namespace Composition
	{
		class CFloorPlan;
	}

	using namespace VisualLinkerScript;
	using namespace VisualLinkerScript::Models;
	using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

	class CModelTranslator
	{
	public:
		/// @brief From a group of Linker-Script files, it generates a CFloorPlan
		static std::shared_ptr<CFloorPlan> TranslateToFloorPlan(const CLinkerScriptFile& linkerScriptFiles);
	};
}

#endif // CMODEL_TRANSLATOR_H__
