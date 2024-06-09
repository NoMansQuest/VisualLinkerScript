#include "QChromeTabButton.h"

void QChromeTabButton::BuildUserInterface()
{
    QIcon icon(":/resources/Images/close-icon.svg");
    QSize iconSize(20, 20);
    QPixmap pixmap = icon.pixmap(iconSize);

    if (!this->m_isFixed)
    {
		this->m_closeButton = new QPushButton(this);
		this->m_closeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		this->m_closeButton->setFixedHeight(16);
		this->m_closeButton->setFixedWidth(16);
		this->m_closeButton->setIcon(pixmap);
        connect(this->m_closeButton, &QPushButton::pressed, this, &QChromeTabButton::CloseButtonPressed);
    }    

    this->m_titleLabel = new QLabel(this->m_displayTitle);
    this->m_titleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->m_titleLabel->setAlignment(Qt::AlignCenter);

    this->m_hboxLayout = new QHBoxLayout;

    this->m_hboxLayout->addWidget(this->m_titleLabel);

    if (!this->m_isFixed)
    {
        this->m_hboxLayout->addWidget(this->m_closeButton);
    }

    this->setLayout(this->m_hboxLayout);
    this->setContentsMargins(0, 0, 0, 0);
    this->m_hboxLayout->setContentsMargins(0, 0, 0, 0);
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
    RefreshStyling();
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

void QChromeTabButton::enterEvent(QEnterEvent* event)
{
	this->m_isHovering = true;
    RefreshStyling();
}

void QChromeTabButton::leaveEvent(QEvent* event)
{
	this->m_isHovering = false;
    RefreshStyling();
}


void QChromeTabButton::RefreshStyling()
{
    if (!this->m_isFixed)
    {
        this->setContentsMargins(0, 0, (this->m_isHovering ? 2 : 24), 0);
        this->m_closeButton->setVisible(this->m_isHovering);
		style()->unpolish(this->m_closeButton);
		style()->polish(this->m_closeButton);
    }
    else
    {
        this->setContentsMargins(0, 0, 13, 0);
    }

    style()->unpolish(this);    
    style()->unpolish(this->m_titleLabel);
	style()->polish(this->m_titleLabel);	
	style()->polish(this);
}

