#include "QChromeTabWidget.h"
#include "QChromeTabButton.h"
#include <utility>

void QChromeTabWidget::BuildUserInterface()
{
    this->m_stackedRegionParent = new QFrame;
    this->m_stackedRegionLayout = new QVBoxLayout;
    this->m_vBoxLayout = new QVBoxLayout(this);    
    this->m_vBoxLayout->setContentsMargins(3, 3, 3, 2);
    
    this->m_scrollAreaForTabButtons = new QScrollArea(this);
    this->m_scrollAreaForTabButtons->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    this->m_scrollAreaForTabButtons->setFixedHeight(28);
    this->m_scrollAreaForTabButtons->setContentsMargins(0, 0, 0, 0);

    this->m_tabButtonsHLayout = new QHBoxLayout;
    this->m_tabButtonsHLayout->setSpacing(0);
    this->m_tabButtonsHLayout->setContentsMargins(0, 0, 0, 0);
    this->m_tabButtonsHLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        
    this->m_scrollAreaForTabButtons->setLayout(this->m_tabButtonsHLayout);    
    this->m_scrollAreaForTabButtons->setContentsMargins(0, 0, 0, 0);    

    this->m_stackedRegionLayout->setContentsMargins(0, 2, 0, 0);
    this->m_stackedRegionParent->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->m_stackedRegionParent->setLayout(this->m_stackedRegionLayout);    

    this->m_vBoxLayout->setSpacing(0);
    this->m_vBoxLayout->addWidget(this->m_scrollAreaForTabButtons);
    this->m_vBoxLayout->addWidget(this->m_stackedRegionParent);   

    this->setLayout(this->m_vBoxLayout);
    this->m_tabIdCounter = 0;
    this->m_currentTabId.reset();    
}

uint32_t QChromeTabWidget::AddTab(std::shared_ptr<QWidget> associatedWidget, bool isFixed)
{
    auto tabId = this->m_tabIdCounter++;
    auto tabHeader = std::shared_ptr<QChromeTabButton>(new QChromeTabButton(tabId, isFixed, this->m_scrollAreaForTabButtons));    
    this->m_tabs[tabId] = std::make_pair(tabHeader, associatedWidget);
    this->m_tabButtonsHLayout->addWidget(tabHeader.get());    
    this->m_tabButtonsHLayout->setSpacing(3);
    this->m_stackedRegionLayout->addWidget(associatedWidget.get());    
    associatedWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    tabHeader->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);        
    tabHeader->setFixedHeight(24);
    connect(tabHeader.get(), &QChromeTabButton::UserRequestedClosure, this, &QChromeTabWidget::CloseButtonClicked);
    connect(tabHeader.get(), &QChromeTabButton::UserRequestedActivation, this, &QChromeTabWidget::TabRequestedActivation);
        
    if (!this->m_currentTabId.has_value())
    {
        this->NavigateToTab(tabId);
    }    
    else
    {
        tabHeader->SetActiveTab(false);
        associatedWidget->setVisible(false);
    }
    
    return tabId;
}

void QChromeTabWidget::RemoveTab(uint32_t tabToRemove)
{
    auto iterator = this->m_tabs.find(tabToRemove);
    if (iterator == this->m_tabs.end())
    {
        throw std::out_of_range("'tabToRemove' out of range.");
    }

    this->m_tabButtonsHLayout->removeWidget(iterator->second.first.get());
    this->m_stackedRegionLayout->removeWidget(iterator->second.second.get());
    this->m_tabs.erase(iterator);

    if (this->m_currentTabId.has_value() && this->m_currentTabId.value() == tabToRemove)
    {
        if (!this->m_tabs.empty())
        {
            this->NavigateToTab(this->m_tabs.begin()->first);
        }
        else
        {
            this->m_currentTabId.reset();
            emit this->ActiveTabChanged(this->m_currentTabId);
        }
    }
}

void QChromeTabWidget::NavigateToTab(uint32_t tabToNavigateTo)
{
    if (this->m_currentTabId.has_value() && this->m_currentTabId.value() == tabToNavigateTo)
    {
        return; // Ignore request, we're already on that tab
    }

    auto iteratorTabToNavigateTo = this->m_tabs.find(tabToNavigateTo);
    if (iteratorTabToNavigateTo == this->m_tabs.end())
    {
        throw std::out_of_range("'tabToNavigateTo' out of range.");
    }

    if  (this->m_currentTabId.has_value())
    {
        auto iterator = this->m_tabs.find(this->m_currentTabId.value());
        if (iterator == this->m_tabs.end())
        {
            throw std::out_of_range("'m_currentTabId' out of range.");
        }

        iterator->second.first->SetActiveTab(false);
    }

    for (auto entry : this->m_tabs)
    {
        entry.second.second->setVisible((entry.first == tabToNavigateTo));
    }
    
    iteratorTabToNavigateTo->second.first->SetActiveTab(true);
    this->m_currentTabId = tabToNavigateTo;
    emit this->ActiveTabChanged(tabToNavigateTo);
}

void QChromeTabWidget::SetTabToolTip(uint32_t targetTab, QString toolTip)
{
    auto iterator = this->m_tabs.find(targetTab);
    if (iterator == this->m_tabs.end())
    {
        throw std::out_of_range("'targetTab' out of range.");
    }

    iterator->second.first->SetToolTip(toolTip);
}

void QChromeTabWidget::SetTabTitle(uint32_t targetTab, QString title)
{
    auto iterator = this->m_tabs.find(targetTab);
    if (iterator == this->m_tabs.end())
    {
        throw std::out_of_range("'targetTab' out of range.");
    }

    iterator->second.first->SetDisplayTitle(title);
}

std::shared_ptr<QWidget> QChromeTabWidget::GetTabContent(uint32_t targetTab)
{
    auto iterator = this->m_tabs.find(targetTab);
    if (iterator == this->m_tabs.end())
    {
        throw std::out_of_range("'targetTab' out of range.");
    }

    return iterator->second.second;
}

std::vector<uint32_t> QChromeTabWidget::Tabs()
{
    std::vector<uint32_t> tabIds;
    for (const auto& entry : this->m_tabs) {
        tabIds.push_back(entry.first);
    }
    return tabIds;
}

std::optional<uint32_t> QChromeTabWidget::CurrentTab()
{
    return this->m_currentTabId;
}
