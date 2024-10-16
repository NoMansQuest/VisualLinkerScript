#ifndef QTREE_VIEW_EX_H__
#define QTREE_VIEW_EX_H__

#include <QTreeView>
#include <QStandardItemModel>
#include <QApplication>
#include <QMouseEvent>

/// @brief Custom tree-view. We need certain overrides.
class QIssuesTreeView final : public QTreeView {
    Q_OBJECT

public:
    /// @brief Default constructor.
    explicit QIssuesTreeView(QWidget* parent = nullptr) : QTreeView(parent)
	{
        // Load icons
        this->m_treeCollapsedIcon = std::make_shared<QIcon>(":/resources/Images/right-arrow-icon.svg");
        this->m_treeExpandedIcon = std::make_shared<QIcon>(":/resources/Images/down-arrow-icon.svg");
        QObject::connect(this, &QTreeView::clicked, this, &QIssuesTreeView::onItemClicked);
        QObject::connect(this, &QTreeView::expanded, this, &QIssuesTreeView::onItemExpanded);
        QObject::connect(this, &QTreeView::collapsed, this, &QIssuesTreeView::onItemCollapsed);
    }

private:
    std::shared_ptr<QIcon> m_treeCollapsedIcon;
    std::shared_ptr<QIcon> m_treeExpandedIcon;

public:
    /// @brief Initializes the root icons.
    void Initialize() const;

private slots:

    /// @brief In this tree view, single-click is needed for collapse/expansion
    void onItemClicked(const QModelIndex& index);

    /// @brief Triggered when item expands.
    void onItemExpanded(const QModelIndex& index) const;

    /// @brief Triggered when item collapsed.
    void onItemCollapsed(const QModelIndex& index) const;
};

#endif
