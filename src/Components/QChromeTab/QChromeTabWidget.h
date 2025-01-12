#ifndef QCHROMETABWIDGET_H__
#define QCHROMETABWIDGET_H__

#include <QtWidgets>
#include <QString>
#include <unordered_map>
#include <memory>
#include <QStackedWidget>

class QChromeTabButton;

/// @brief The main tab component to emulate google chrome's tab component.
class QChromeTabWidget : public QFrame
{
    Q_OBJECT

private:
    QFrame* m_stackedRegionParent;
    QVBoxLayout* m_stackedRegionLayout;
    QVBoxLayout* m_vBoxLayout;
    QScrollArea* m_scrollAreaForTabButtons;
    QHBoxLayout* m_tabButtonsHLayout;
    std::atomic<uint32_t> m_tabIdCounter;
    std::optional<uint32_t> m_currentTabId;
    std::unordered_map<uint32_t, std::pair<std::shared_ptr<QChromeTabButton>, std::shared_ptr<QWidget>>> m_tabs;

public:
    /// @brief Default constructor
    QChromeTabWidget(QWidget *parent = 0)
        : QFrame(parent)
    {            
        this->BuildUserInterface();
    }

    /// @brief Default destructor
    ~QChromeTabWidget()
    {}

public:
    /// @brief Add the tab to the list
    uint32_t AddTab(const std::shared_ptr<QWidget>& associatedWidget, bool isFixed = false);

    /// @brief Remove the given tab.
    void RemoveTab(uint32_t tabId);

    /// @brief Navigate to the selected tab and have it activated.
    void NavigateToTab(uint32_t tabId);

    /// @brief Updates tab's tool tip
    void SetTabToolTip(uint32_t tabId, const QString& toolTip);

    /// @brief Updates tab's title
    void SetTabTitle(uint32_t tabId, const QString& title);

    /// @brief Updates a given tabs closure policy.
    void SetTabClosurePolicy(uint32_t targetTabId, bool isClosureAllowed);

    /// @brief Returns associated content of a given tab.
    std::shared_ptr<QWidget> GetTabContent(uint32_t tabId) const;

    /// @brief Returns back a list of all the present tabs.
    std::vector<uint32_t> Tabs() const;

    /// @brief Reports back the current active tab.
    std::optional<uint32_t> CurrentTabId() const;

    /// @brief Reports back the current active tab content.
    std::shared_ptr<QWidget> CurrentTab() const;

    /// @brief Returns the available number of tabs.
    uint32_t TabCount() const { return this->m_tabs.size(); }

signals:
    /// @brief Triggered when user selects a different tab
    /// @param activeTab An optional ID of the tab that is currently active. If no tabs are present, the activeTab will contain no value.
    void evActiveTabChanged(std::optional<uint32_t> activeTab);

    /// @brief User has pressed the 'close' button of the tab.
    /// @param requestedTabToClose Id of the tab user has requested to close.
    void evUserRequestedTabClosure(uint32_t requestedTabToClose);

    /// @brief Triggered when the tabs collections changes (either tabs added or removed).
    void evTabsCollectionUpdated();

protected slots:
    void CloseButtonClicked(uint32_t tabId) { emit evUserRequestedTabClosure(tabId); }
    void TabRequestedActivation(uint32_t tabId) { this->NavigateToTab(tabId); }

protected:
    /// @brief Overridden as this is where we paint the object
    void BuildUserInterface();

    /// @brief Recalculates tab location, height, etc.
    void RecalculateGraphicCoordinates();
};

#endif
