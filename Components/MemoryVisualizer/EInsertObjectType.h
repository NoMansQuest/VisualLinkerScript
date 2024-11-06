#ifndef EINSERT_OBJECT_TYPE_H__
#define EINSERT_OBJECT_TYPE_H__

/// @brief Type of object user is asking to insert
enum class EInsertObjectType
{
    MemoryObject,
    PointerAssignmentStatement,
    SymbolAssignmentStatement,
    SectionObject,
    SectionOutput,
};

#endif // EINSERT_OBJECT_TYPE_H__
