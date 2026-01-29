#include "Components/QLinkerScriptSession/CustomComponents/QIssuesTreeView.h"

void QIssuesTreeView::Initialize()
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
    if (!model) 
    {
        return; 
    }

    for (int row = 0; row < model->rowCount(); ++row) 
    {
	    if (auto rootItem = model->item(row)) 
        {
            rootItem->setIcon(this->isExpanded(rootItem->index()) ?
				*this->m_treeExpandedIcon :
				*this->m_treeCollapsedIcon);
        }
    }

    this->setSelectionBehavior(SelectionBehavior::SelectRows);
}

void QIssuesTreeView::onItemClicked(const QModelIndex& index)
{
    if (index.parent().isValid()) // Only the root items could be expanded/collapsed.
    {
        return;
    }

    if (this->isExpanded(index))
    {
        this->collapse(index);
    }
    else
    {
        this->expand(index);
    }
}

void QIssuesTreeView::onItemCollapsed(const QModelIndex& index) const
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
    if (!model || index.parent().isValid())
    {
        return;
    }

    QStandardItem* item = model->itemFromIndex(index);
    item->setIcon(*this->m_treeCollapsedIcon);
}

void QIssuesTreeView::onItemExpanded(const QModelIndex& index) const
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
    if (!model || index.parent().isValid())
    {
        return;
    }

    QStandardItem* item = model->itemFromIndex(index);
    item->setIcon(*this->m_treeExpandedIcon);
}
