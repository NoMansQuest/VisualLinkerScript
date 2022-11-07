#ifndef __LINKER_SCRIPT__
#define __LINKER_SCRIPT__

namespace VisualLinkerScript::Models::LinkerScript
{   
    class CMemoryRegion;
    class CProgramHeadersRegion;    
    class CSectionsRegion;

    class CLinkerScriptEntity :
    {   
    public:
        CLinkerScriptEntity();

    public:
        CProgramHeadersRegion Phdrs;
        CMemoryRegion Memories;
        CSesionsRegion Sections;
    }
}

#endif