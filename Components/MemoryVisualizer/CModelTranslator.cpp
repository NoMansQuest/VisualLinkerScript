#include "CModelTranslator.h"

#include "Composition/CFloorPlan.h"

using namespace VisualLinkerScript::Components::MemoryVisualizer;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

/// @copydoc
std::shared_ptr<CFloorPlan> CModelTranslator::TranslateToFloorPlan(const CLinkerScriptFile& linkerScriptFile)
{
	/*
	QFont font("Tahoma, DejaVu Sans", 12);
	font.setFamily("Arial, Helvetica, sans-serif"); // Specify primary and fallback fonts
	QFontMetrics fontMetrics(font);
	*/

    auto translatedFloorPlan = std::make_shared<CFloorPlan>(SharedPtrVector<CMemorySection>());

    /*
    for (auto lsFile : linkerScriptFiles) {
        lsFile->

    }*/
    return translatedFloorPlan;
}


