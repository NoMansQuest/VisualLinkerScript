#include "QChromeTabButton.h"

void QChromeTabButton::BuildUserInterface()
{
    QIcon icon(":/LocalResources/close-sm-svgrep-com.svg");
    QSize iconSize(20, 20);
    QPixmap pixmap = icon.pixmap(iconSize);

    this->m_closeButton = new QPushButton(this);
    this->m_closeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->m_closeButton->setFixedHeight(30);
    this->m_closeButton->setFixedWidth(30);
    this->m_closeButton->setIcon(pixmap);

    this->m_titleLabel = new QLabel(this->m_displayTitle, this);
    this->m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    this->m_hboxLayout = new QHBoxLayout(this);
    this->setLayout(this->m_hboxLayout);

    this->m_hboxLayout->addWidget(this->m_titleLabel);
    this->m_hboxLayout->addWidget(this->m_closeButton);
    this->m_hboxLayout->setContentsMargins(20, 20, 20, 10);

    connect(this->m_closeButton, &QPushButton::pressed, this, &QChromeTabButton::CloseButtonPressed);
}

void QChromeTabButton::SetDisplayTitle(QString displayTitle)
{
    this->m_displayTitle = displayTitle;
    this->m_titleLabel->setText(displayTitle);
}

void QChromeTabButton::SetToolTip(QString toolTip)
{
    this->m_toolTip = toolTip;
    this->setToolTip(toolTip);
    this->m_titleLabel->setToolTip(toolTip);
}

void QChromeTabButton::SetActiveTab(bool activeTab)
{    
    this->m_isActiveTab = activeTab;
}

void QChromeTabButton::CloseButtonPressed()
{
    emit this->UserRequestedClosure(this->m_tabId);
}

void QChromeTabButton::mousePressEvent(QMouseEvent* event)
{
    if (!this->m_isActiveTab)
    {
        emit this->UserRequestedActivation(this->m_tabId);
    }
    QWidget::mousePressEvent(event); // Ensure default handling of the event
}

