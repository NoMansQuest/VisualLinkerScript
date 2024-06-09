#ifndef MODELTRANSLATOR_H__
#define MODELTRANSLATOR_H__

#include <vector>
#include <memory>
#include "../../Helpers.h"
#include "../../Models/CLinkerScriptFile.h"
#include "Models/CFloorplan.h"

using namespace VisualLinkerScript;
namespace ProjectModels = VisualLinkerScript::Models;

/// @brief From a group of Linker-Script files, it generates a CFloorplan
std::shared_ptr<CFloorplan> Translate(SharedPtrVector<ProjectModels::CLinkerScriptFile> linkerScriptFiles);

#endif // MODELTRANSLATOR_H__
