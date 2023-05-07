#ifndef __LINKER_SCRIPT__
#define __LINKER_SCRIPT__

#include "CProgramHeaderRegion.h"
#include "CMemoryRegion.h"
#include "CSectionsRegion.h"

namespace VisualLinkerScript::Models::LinkerScript
{

    class CLinkerScriptEntity
    {   

    public:
        CLinkerScriptEntity();

    public:
        CProgramHeadersRegion Phdrs;
        CMemoryRegion Memories;
        CSectionsRegion Sections;
    };
}

#endif
