#ifndef QCHROMETABBUTTON_H
#define QCHROMETABBUTTON_H

#include <QFrame>
#include <QtWidgets>
#include <QString>
#include <QMouseEvent>
#include <QLabel>
#include <QIcon>
#include <memory>

/// @brief The QChromeTab is composed of QChromeTabItem objects
class QChromeTabButton : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool IsHighlighted READ IsHighlighted USER true)
    Q_PROPERTY(bool IsActiveTab READ IsActiveTab NOTIFY ActiveTabStateChanged USER true)
    Q_PROPERTY(bool IsHovering READ IsHovering USER true)
    Q_PROPERTY(QString DisplayTitle READ DisplayTitle WRITE SetDisplayTitle NOTIFY DisplayTitleChanged USER true)

private:
    QString m_displayTitle;
    bool m_isHighlighted;
    bool m_isActiveTab;
    bool m_isFixed;
    bool m_isHovering;
    QString m_toolTip;
    uint32_t m_tabId;
    std::shared_ptr<QWidget> m_associatedContent;

    // Objects
    QHBoxLayout* m_hboxLayout;
    QPushButton* m_closeButton;
    QLabel* m_titleLabel;

public:
    /// @brief Default constructor
    QChromeTabButton(uint32_t tabId, bool isFixed, QWidget *parent = nullptr)
        : QFrame(parent),
            m_displayTitle(""),
            m_toolTip(""),
            m_isHovering(false),
            m_isActiveTab(false),
            m_isFixed(isFixed),
            m_tabId(tabId)
    {
        this->setMouseTracking(true);
        this->BuildUserInterface();
    }

    /// @brief Default destructor
    ~QChromeTabButton()
    {}

signals:
    void UserRequestedActivation(uint32_t tabId);
    void UserRequestedClosure(uint32_t tabId);
    void ActiveTabStateChanged();
    void DisplayTitleChanged();

protected slots:
    void CloseButtonPressed();
    void RefreshStyling();

public:
    QString DisplayTitle() { return this->m_displayTitle; }
    QString ToolTip() { return this->m_toolTip; }
    bool IsActiveTab() { return this->m_isActiveTab; }
    bool IsHighlighted() { return this->m_isHighlighted; }
    bool IsHovering() { return this->m_isHovering; }

    void SetDisplayTitle(QString displayTitle);
    void SetToolTip(QString toolTip);
    void SetActiveTab(bool activeTab);

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void BuildUserInterface();
};


#endif // QCHROMETABBUTTON_H
