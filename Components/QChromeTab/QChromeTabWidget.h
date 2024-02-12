#ifndef QCHROMETABWIDGET_H__
#define QCHROMETABWIDGET_H__

#include <QtWidgets>
#include <QString>
#include <vector>
#include <memory>
#include <QStackedWidget>

namespace VisualLinkerScript::Components
{
    /// @brief The main tab component to emulate google chrome's tab compoonent.
    class QChromeTabWidget : public QWidget
    {
        Q_OBJECT

    private:
        QStackedWidget* m_stackedWidget;
        QVBoxLayout* m_vBoxLayout;
        QScrollArea* m_scrollAreaForTabButtons;

        std::vector<std::shared_ptr<QWidget>> m_tabs;

    public:
        /// @brief Default constructor
        QChromeTabWidget(QWidget *parent = 0)
            : QWidget(parent)
        {            
            this->BuildUserInterface();
        }

        /// @brief Default desctructor
        ~QChromeTabWidget()
        {}

    public:
        /// @brief Add the tab to the list
        void AddTab(std::shared_ptr<QWidget> newTab);

        /// @brief Remove the given tab.
        void RemoveTab(std::shared_ptr<QWidget> tabToRemove);

        /// @brief Navigate to the selected tab and have it activated.
        void NavigateToTab(std::shared_ptr<QWidget> taToNavigateTo);

        /// @brief Returns back a list of all the present tabs.
        std::vector<std::shared_ptr<QWidget>> Tabs();

        /// @brief Reports back the current active tab.
        std::shared_ptr<QWidget> CurrentTab();

    signals:
        /// @brief Triggered when user selects a different tab
        void ActiveTabChanged(std::shared_ptr<QWidget> activeTab);

        /// @brief User has pressed the 'close' button of the tab.
        void UserRequestedTabClosure(std::shared_ptr<QWidget> requestedTabToClose);


    protected:
        /// @brief Overriden as this is where we paint the object
        void BuildUserInterface();

        /// @brief Recalculates tab location, height, etc.
        void RecalculateGraphicCoordinates();
    };
};


#endif
