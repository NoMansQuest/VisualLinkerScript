#ifndef MODELTRANSLATOR_H__
#define MODELTRANSLATOR_H__

#include <vector>
#include <memory>
#include "../../Models/CLinkerScriptFile.h"
#include "Models/CFloorplan.h"

namespace MemoryVisualizerModels = VisualLinkerScript::Components::MemoryVisualizer::Models;
namespace ProjectModels = VisualLinkerScript::Models;

namespace VisualLinkerScript::MemoryVisualizer
{
    /// @brief From a group of Linker-Script files, it generates a CFloorplan
    std::shared_ptr<MemoryVisualizerModels::CFloorplan> Translate(std::vector<std::shared_ptr<ProjectModels::CLinkerScriptFile>> linkerScriptFiles);
};


#endif // MODELTRANSLATOR_H__
