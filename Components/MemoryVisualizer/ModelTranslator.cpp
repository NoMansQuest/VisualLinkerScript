#include "ModelTranslator.h"

using namespace VisualLinkerScript::MemoryVisualizer;

/// @copydoc
std::shared_ptr<MemoryVisualizerModels::CFloorplan> Translate(std::vector<std::shared_ptr<ProjectModels::CLinkerScriptFile>> linkerScriptFiles)
{

    auto translatedFloorplan = std::make_shared<MemoryVisualizerModels::CFloorplan>();

    for (auto lsFile : linkerScriptFiles)
    {
        lsFile->

    }

    return translatedFloorplan;
}


