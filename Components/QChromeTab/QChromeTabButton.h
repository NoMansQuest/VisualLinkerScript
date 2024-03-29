#ifndef QCHROMETABBUTTON_H
#define QCHROMETABBUTTON_H

#include <QtWidgets>
#include <QString>
#include <QtWidgets>
#include <QLabel>
#include <QIcon>
#include <memory>

namespace VisualLinkerScript::Components
{
    /// @brief The QChromTab is composed of CChromeTabItem objects
    class QChromeTabButton : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(bool IsHighlighted READ IsHighlighted USER true)
        Q_PROPERTY(bool IsActiveTab READ IsActiveTab USER true)
        Q_PROPERTY(QString DisplayTitle READ DisplayTitle WRITE SetDisplayTitle USER true)

    private:
        QString m_displayTitle;
        bool m_isHighlighted;
        bool m_isActiveTab;
        QString m_toolTip;
        uint32_t m_tabId;
        std::shared_ptr<QWidget> m_associatedContent;

        // Objects
        QHBoxLayout* m_hboxLayout;
        QPushButton* m_closeButton;
        QLabel* m_titleLabel;

    public:
        /// @brief Default constructor
        QChromeTabButton(uint32_t tabId, QWidget *parent = nullptr)
            : QWidget(parent),
              m_displayTitle(""),
              m_toolTip(""),
              m_tabId(tabId)
        {
            this->BuildUserInterface();
        }

        /// @brief Default desctructor
        ~QChromeTabButton()
        {}

    signals:
        void UserRequestedActivation(uint32_t tabId);
        void UserRequestedClosure(uint32_t tabId);

    protected slots:
        void CloseButtonPressed();

    public:
        QString DisplayTitle() { return this->m_displayTitle; }
        QString ToolTip() { return this->m_toolTip; }
        bool IsActiveTab() { return this->m_isActiveTab; }
        bool IsHighlighted() { return this->m_isHighlighted; }

        void SetDisplayTitle(QString displayTitle);
        void SetToolTip(QString toolTip);
        void SetActiveTab(bool activeTab);

    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void BuildUserInterface();
    };
};


#endif // QCHROMETABBUTTON_H
