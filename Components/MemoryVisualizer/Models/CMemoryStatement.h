#ifndef CMEMORYSTATEMENT_H
#define CMEMORYSTATEMENT_H

#include <vector>
#include <string>
#include <cstdint>
#include "../../../Helpers.h"
#include "CSizableObject.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents a Memory-Statement.
    class CMemoryStatement : public CSizableObject
    {
    public:
        /// @brief Default constructor
        CMemoryStatement(bool isExternal)
            : CSizableObject(true, isExternal)
        {}

    private:
        std::string m_title;
        std::string m_fillExpression;
        std::vector<std::string> m_phdrList;
        SharedPtrVector<CContentBase> m_childContent;

    public:
        std::string Title(){
            return this->m_title;
        }

        void SetTitle(std::string value){
            this->m_title = value;
        }

        std::string


    };
};

#endif // CMEMORYSTATEMENT_H
