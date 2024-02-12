#ifndef QCHROMETABBUTTON_H
#define QCHROMETABBUTTON_H

#include <QtWidgets>
#include <QString>
#include <QtWidgets>
#include <QLabel>
#include <QIcon>
#include <memory>

namespace VisualLinkerScript::Components::QChromeTab
{
    /// @brief The QChromTab is composed of CChromeTabItem objects
    class QChromeTabButton : QWidget
    {
        Q_OBJECT
        Q_PROPERTY(bool IsHighlighted READ IsHighlighted USER true)
        Q_PROPERTY(bool IsActiveTab READ IsActiveTab USER true)
        Q_PROPERTY(QString  DisplayTitle READ DisplayTitle WRITE SetDisplayTitle USER true)
        Q_PROPERTY(bool IsFileOnDisk READ IsFileOnDisk WRITE SetFileOnDisk USER true)

    private:
        QString m_displayTitle;
        bool m_isHighlighted;
        bool m_isActiveTab;
        bool m_isFileOnDisk;
        QString m_filePath;
        std::shared_ptr<QWidget> m_associatedContent;

        // Objects
        QHBoxLayout* m_hboxLayout;
        QPushButton* m_closeButton;
        QLabel* m_titleLabel;

    public:
        /// @brief Default constructor
        QChromeTabButton(std::shared_ptr<QWidget> associatedContent,
                         QString displayTitle,
                         bool isFileOnDisk,
                         QWidget *parent = nullptr)
            : QWidget(parent),
              m_displayTitle(displayTitle),
              m_isFileOnDisk(isFileOnDisk),
              m_associatedContent(associatedContent)
        {
            this->BuildUserInterface();
        }

        /// @brief Default desctructor
        ~QChromeTabButton()
        {}

    signals:
        void UserPressed();
        void UserRequestedClosure();

    public:
        QString DisplayTitle() { return this->m_displayTitle; }
        bool IsFileOnDisk() { return this->m_isFileOnDisk; }
        QString FilePath() { return this->m_filePath; }
        bool IsActiveTab() { return this->m_isActiveTab; }
        bool IsHighlighted() { return this->m_isHighlighted; }
        std::shared_ptr<QWidget> AssociatedContent() { return this->m_associatedContent; }

        void SetDisplayTitle(QString displayTitle);
        void SetFileOnDisk(bool fileOnDisk);
        void SetFilePath(QString filePath);
        void SetActiveTab(bool activeTab);

    private:
        void BuildUserInterface();
    };
};


#endif // QCHROMETABBUTTON_H
