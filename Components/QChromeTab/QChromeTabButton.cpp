#include "QChromeTabButton.h"

using namespace VisualLinkerScript::Components::QChromeTab;

void QChromeTabButton::BuildUserInterface()
{
    QIcon icon(":/LocalResources/close-sm-svgrep-com.svg");
    QSize iconSize(20, 20);
    QPixmap pixmap = icon.pixmap(iconSize);

    this->m_closeButton = new QPushButton(this);
    this->m_closeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->m_closeButton->setFixedHeight(30);
    this->m_closeButton->setFixedHeight(30);
    this->m_closeButton->setIcon(pixmap);

    this->m_titleLabel = new QLabel(this->m_displayTitle, this);
    this->m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    this->m_hboxLayout = new QHBoxLayout(this);
    this->setLayout(this->m_hboxLayout);

    this->m_hboxLayout->addWidget(this->m_titleLabel);
    this->m_hboxLayout->addWidget(this->m_closeButton);
    this->m_hboxLayout->setContentsMargins(20, 20, 20, 10);
}

void QChromeTabButton::SetDisplayTitle(QString displayTitle)
{
    this->m_displayTitle = displayTitle;
    this->m_titleLabel->setText(displayTitle);
}

void QChromeTabButton::SetFileOnDisk(bool fileOnDisk)
{
    this->m_isFileOnDisk = fileOnDisk;
}

void QChromeTabButton::SetFilePath(QString filePath)
{
    this->m_filePath = filePath;
}

void QChromeTabButton::SetActiveTab(bool activeTab)
{
    this->m_isActiveTab = activeTab;
}
