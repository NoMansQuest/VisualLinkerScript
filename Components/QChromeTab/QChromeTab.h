#ifndef QCHROMETAB_H__
#define QCHROMETAB_H__

#include <QtWidgets>
#include <QString>
#include <vector>
#include <memory>

namespace VisualLinkerScript::Components::QChromeTab
{
    class CChromeTabItem;

    /// @brief The main tab component to emulate google chrome's tab compoonent.
    class QChromeTab : public QWidget
    {
        Q_OBJECT

    private:
        std::vector<std::shared_ptr<CChromeTabItem>> m_tabs;
        float m_xscroll_offset;

    public:
        /// @brief Default constructor
        QChromeTab(QWidget *parent = 0)
            : QWidget(parent)
        { }

        /// @brief Default desctructor
        ~QChromeTab()
        {}

    public:
        /// @brief Add the tab to the list
        void AddTab(CChromeTabItem newTabItem);

        /// @brief Remove the given tab.
        void RemoveTab(std::shared_ptr<CChromeTabItem> targetTab);

        /// @brief Navigate to the selected tab and have it activated.
        void NavigateToTab(std::shared_ptr<CChromeTabItem> tabToNavigateTo);

    signals:
        /// @brief Triggered when user selects a different tab
        void ActiveTabChanged(std::shared_ptr<CChromeTabItem> activeTab);

        /// @brief User has pressed the 'close' button of the tab.
        void UserRequestedTabClosure(std::shared_ptr<CChromeTabItem> tab);

    protected:
        /// @brief Overriden as this is where we paint the object
        void paintEvent(QPaintEvent* paintEvent) Q_DECL_OVERRIDE;

    };
};


#endif
