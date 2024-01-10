#ifndef QCHROMETABITEM_H
#define QCHROMETABITEM_H

#include <QtWidgets>
#include <QString>

namespace VisualLinkerScript::Components::QChromeTab
{
    /// @brief The QChromTab is composed of QChromeTabItem objects
    class QChromeTabItem : public QObject
    {
        Q_OBJECT

    public:
        /// @brief Default constructor
        QChromeTabItem()
        {}

        /// @brief Default desctructor
        ~QChromeTabItem()
        {}

    private:
        QString m_title;
        QString m_filePath;
        bool m_isFileSaved;
        QRectF m_clientArea;
        bool m_isHighlighted;
        QString m_displayTitle;
        QRectF m_closeButtonClientArea;
        bool m_isCloseButtonHighlighted;
        bool m_isFloating;
        QPointF m_dragStartPoint;
        QRectF m_provisionalClientArea;
        QRectF m_provisionalCloseButtonClientArea;
        bool m_isActiveTab;

    public:
        QString DisplayTitle() { return this->m_displayTitle; }
        bool IsFileSaved() { return this->m_isFileSaved; }
        QString FilePath() { return this->m_filePath; }
        bool IsActiveTab() { return this->m_isActiveTab; }

        void SetDisplayTitle(QString displayTitle) { this->m_displayTitle = displayTitle; }
        void SetFileSaved(bool fileSaved) { this->m_isFileSaved = fileSaved; }
        void SetFilePath(QString filePath) { this->m_filePath = filePath; }

    protected:
        bool IsHighlighted(){ return this-m_isHighlighted; }
        QRectF ProvisionalClientArea() { return this->m_provisionalClientArea; }
        QRectF ClientArea() { return this->m_clientArea; }
        QRectF CloseButtonClientArea() { return this->m_closeButtonClientArea; }
        QRectF ProvisionalCloseButtonClientArea() { return this->m_provisionalCloseButtonClientArea; }
        bool IsCloseButtontHighlighted() { return this->m_isCloseButtonHighlighted; }
        bool IsFloating() { return this->m_isFloating; }

        void SetIsHighlighted(bool isHighlighted) { this->m_isHighlighted = isHighlighted; }
        void SetProvisionalClientArea(QRectF area) { this->m_provisionalClientArea = area; }
        void SetClientArea(QRectF clientArea) { this->m_clientArea = clientArea; }
        void SetCloseButtonClientArea(QRectF area) { this->m_closeButtonClientArea = area; }
        void SetCloseButtonHighlighted(bool highlighted) { this->m_isCloseButtonHighlighted = highlighted; }
        void SetProvisionalCloseButtonClientArea(QRectF area) { this->m_provisionalCloseButtonClientArea = area;}
        void SetFloating(bool floating) { this->m_isFloating = floating; }
        void SetActiveTab(bool activeTab) { this->m_isActiveTab = activeTab; }
    };
};


#endif // QCHROMETABITEM_H
