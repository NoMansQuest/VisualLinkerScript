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
    uint32_t AddTab(std::shared_ptr<QWidget> associatedWidget);

    /// @brief Remove the given tab.
    void RemoveTab(uint32_t tabToRemove);

    /// @brief Navigate to the selected tab and have it activated.
    void NavigateToTab(uint32_t tabToNavigateTo);

    /// @brief Updates tab's tool tip
    void SetTabToolTip(uint32_t targetTab, QString toolTip);

    /// @brief Updates tab's title
    void SetTabTitle(uint32_t targetTab, QString title);

    /// @brief Returns associated content of a given tab.
    std::shared_ptr<QWidget> GetTabContent(uint32_t targetTab);

    /// @brief Returns back a list of all the present tabs.
    std::vector<uint32_t> Tabs();

    /// @brief Reports back the current active tab.
    std::optional<uint32_t> CurrentTab();

signals:
    /// @brief Triggered when user selects a different tab
    /// @param activeTab An optional ID of the tab that is currently active. If no tabs are present, the activeTab will contain no value.
    void ActiveTabChanged(std::optional<uint32_t> activeTab);

    /// @brief User has pressed the 'close' button of the tab.
    /// @param requestedTabToClose Id of the tab user has requested to close.
    void UserRequestedTabClosure(uint32_t requestedTabToClose);

protected slots:
    void CloseButtonClicked(uint32_t tabId) { emit UserRequestedTabClosure(tabId); }
    void TabRequestedActivation(uint32_t tabId) { this->NavigateToTab(tabId); }

protected:
    /// @brief Overridden as this is where we paint the object
    void BuildUserInterface();

    /// @brief Recalculates tab location, height, etc.
    void RecalculateGraphicCoordinates();
};

#endif
