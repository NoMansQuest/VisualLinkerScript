#ifndef MODELTRANSLATOR_H__
#define MODELTRANSLATOR_H__

#include <vector>
#include <memory>
#include "../../Helpers.h"
#include "../../Models/CLinkerScriptFile.h"
#include "Models/CFloorplan.h"

namespace MemoryVisualizerModels = VisualLinkerScript::Components::MemoryVisualizer::Models;
namespace ProjectModels = VisualLinkerScript::Models;

namespace VisualLinkerScript::MemoryVisualizer
{
    using namespace VisualLinkerScript;

    /// @brief From a group of Linker-Script files, it generates a CFloorplan
    std::shared_ptr<MemoryVisualizerModels::CFloorplan> Translate(SharedPtrVector<ProjectModels::CLinkerScriptFile> linkerScriptFiles);
};


#endif // MODELTRANSLATOR_H__
