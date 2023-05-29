#ifndef EINSERTOBJECTTYPE_H__
#define EINSERTOBJECTTYPE_H__

namespace VisualLinkerScript::Components::MemoryVisualizer
{
    /// @brief Type of object user is asking to insert
    enum class EInsertObjectType
    {
        MemoryObject,
        PointerAssignmentStatement,
        SymbolAssignmentStatement,
        SectionObject,
        SectionOutput,
    };
};

#endif // EINSERTOBJECTTYPE_H
