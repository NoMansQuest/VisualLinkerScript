#ifndef CPHDRS_REGION_PARSER_H__
#define CPHDRS_REGION_PARSER_H__

#include "CSubParserBase.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{   
    /* PHDRS Region has the following format:
     *
     * PHDRS 
     * {
     *    name type [ FILEHDR ] [ PHDRS ] [ AT ( address ) ] [ FLAGS ( flags ) ] ;
     * }
     * 
     * 
     * @ "type" can have the following potential values: 
     * 
     * PT_NULL (0):    Indicates an unused program header.
     * PT_LOAD (1):    Indicates that this program header describes a segment to be loaded from the file.
     * PT_DYNAMIC (2): Indicates a segment where dynamic linking information can be found.
     * PT_INTERP (3):  Indicates a segment where the name of the program interpreter may be found.
     * PT_NOTE (4):    Indicates a segment holding note information.
     * PT_SHLIB (5):   A reserved program header type, defined but not specified by the ELF ABI.
     * PT_PHDR (6):    Indicates a segment where the program headers may be found.
     * PT_TLS (7):     Indicates a segment containing thread local storage.
     * 
     * 
     * @ The FILEHDR keyword means that the segment should include the ELF file header. 
     * @ The PHDRS keyword means that the segment should include the ELF program headers themselves. 
     *   -> If applied to a loadable segment (PT_LOAD), all prior loadable segments must have one of these keywords.
     * @ The AT command used as an output section attribute. The AT command for a program header overrides the output section attribute.
     * @ The value of flags must be an integer. It is used to set the p_flags field of the program header.
     * 
     */

    /// @brief Object in charge of parsing the "PHDRS" region inside a linker-script
    class CPhdrsRegionParser : CSubParserBase
    {
    public:
        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::vector<CLinkerScriptContentBase>&& TryParse(std::vector<CRawEntry>::const_iterator& iterator) override;
    }
}

#endif