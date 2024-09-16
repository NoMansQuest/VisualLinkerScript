#include "QViolationsModel.h"
#include "CViolationEntryBase.h"

QViolationsModel::QViolationsModel(const QString& data, QObject* parent)
    : QAbstractItemModel(parent)
{}

int QViolationsModel::columnCount(const QModelIndex& parent) const
{
    return this->rootItem->ColumnCount();
}

QVariant QViolationsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    const auto violationEntry = static_cast<CViolationEntryBase*>(index.internalPointer());

    if (role == Qt::DisplayRole)
    {
        return violationEntry->Data(index.column());
    }

    return QVariant();
}

Qt::ItemFlags QViolationsModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    return QAbstractItemModel::flags(index);
}

QVariant QViolationsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return this->rootItem->Data(section);
    }

    return QVariant();
}

QModelIndex QViolationsModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    CViolationEntryBase* parentItem;

    if (!parent.isValid())
    {
        parentItem = rootItem;
    }
    else
    {
        parentItem = static_cast<CViolationEntryBase*>(parent.internalPointer());
    }

    if (auto childItem = parentItem->Child(row))
    {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex QViolationsModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    const auto childItem = static_cast<CViolationEntryBase*>(index.internalPointer());
    const auto parentItem = childItem->Parent();

    if (parentItem == rootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->Row(), 0, parentItem);
}

int QViolationsModel::rowCount(const QModelIndex& parent) const
{
    CViolationEntryBase* parentItem;
    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = rootItem;
    }
    else
    {
        parentItem = static_cast<CViolationEntryBase*>(parent.internalPointer());
    }

    return parentItem->ChildCount();
}