#ifndef VIOLATION_ENTRY_BASE_H_
#define VIOLATION_ENTRY_BASE_H_

#include <QVariant>
#include <QStringList>
#include <utility>
#include <quuid.h>

/// @brief Entity represents the base class for a violation. These violations must be specialized prior use.
class CViolationEntryBase
{

public:
    /// @brief Creates an instance of CViolationEntryBase.
    CViolationEntryBase(
        QUuid uuid,
        std::string violationCode,
        std::string description,
        std::string affectedFile,
        const uint32_t lineNumber,
        const uint32_t columnNumber,
        CViolationEntryBase* parent = nullptr)     
        : m_uuid(uuid),
	      m_violationCode(std::move(violationCode)),
          m_description(std::move(description)),
          m_affectedFile(std::move(affectedFile)),
          m_lineNumber(lineNumber),
          m_columnNumber(columnNumber),
          m_parentItem(std::move(parent))
	{}

    /// @breif Default destructor.
    ~CViolationEntryBase()
    {
        qDeleteAll(this->m_childItems);
    }

    /// @brief Adds a child in form of a shared pointer.
    void AppendChild(CViolationEntryBase* child)
    {
        m_childItems.append(child);
    }

    /// @brief Returns the unique identifier of this entry.
    [[nodiscard]] QUuid Uuid() const
    {
        return this->m_uuid;
    }

    /// @brief Returns child based on the given index.
    /// @param row Row index of the child.
    /// @return The child at the given index.
    [[nodiscard]] CViolationEntryBase* Child(uint32_t row)
    {
	    return this->m_childItems.value(row);
    }

    /// @brief Reports back the number of children this entry contains.
    [[nodiscard]] uint32_t ChildCount() const noexcept
    {
	    return this->m_childItems.count();
    }

    /// @breif Reports back the number of columns this object has.
    [[nodiscard]] uint32_t ColumnCount() const noexcept
    {
        return 5; // We have five columns
    }

    /// @brief Reports back column data.
    /// @param column Colum index.
    /// @return Returns the value the sub-item holds at the given column.
    [[nodiscard]] QVariant Data(uint32_t column) const
    {
	    switch (column)
	    {
	    	case 0: return this->m_violationCode.data();
	    	case 1: return this->m_description.data();
	    	case 2: return this->m_affectedFile.data();
	    	case 3: return this->m_lineNumber;
	    	case 4: return this->m_columnNumber;
	    	default:
                throw std::invalid_argument("Column value was found to be out of range.");
	    }
    }

    /// @brief Reports back the row index of this entry in parent.
    [[nodiscard]] uint32_t Row() const
	{
        if (this->m_parentItem == nullptr)
        {
            return 0;
        }

        for (uint32_t index = 0; index < this->m_parentItem->m_childItems.count(); index++)
        {
	        if (this->m_parentItem->m_childItems[index]->Uuid() == this->m_uuid)
	        {
                return index;
	        }
        }
        throw std::out_of_range("Parent does not contain this entry!");
    }

    /// @brief Reports back the parent of this node.
    [[nodiscard]] CViolationEntryBase* Parent()
    {
	    return this->m_parentItem;
    }

private:
    QUuid m_uuid;
    std::string m_violationCode;
    std::string m_description;
    std::string m_affectedFile;
    uint32_t m_lineNumber;
    uint32_t m_columnNumber;

    QList<CViolationEntryBase*> m_childItems;    
    CViolationEntryBase* m_parentItem;
};

#endif