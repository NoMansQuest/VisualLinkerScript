#ifndef QLINKERSCRIPT_PANEL_H__
#define QLINKERSCRIPT_PANEL_H__

#include <QListView>
#include <QGridLayout>
#include <QSplitter>
#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QUuid>
#include "../Components/QScintilla/src/Qsci/qsciscintilla.h"

namespace VisualLinkerScript::Components::MemoryVisualizer
{
    class QMemoryVisualizer;
};

namespace VisualLinkerScript::Widgets
{
    using namespace VisualLinkerScript::Components::MemoryVisualizer;

    class QLinkerScriptPanel : public QWidget
    {
        Q_OBJECT

    // Model related fields
    private:
        /// @brief When the file is not saved yet, this will be something like 'Untitled (1)', 'Untitled (2)'), etc.
        ///        However, once the file is saved, this would be identical to the name of the file.
        QString m_title;
        QString m_filePath;
        QUuid m_sessionUuid;
        bool m_isNewFile;

    public:
        /// @brief Default constructor
        QLinkerScriptPanel(QString initialTitle,
                           QUuid sessionUuid,
                           QWidget *parent = nullptr)
            : m_title(initialTitle),
              m_filePath(""),
              m_sessionUuid(sessionUuid),
              m_isNewFile(false)
        {}

        /// @brief Default destructor
        ~QLinkerScriptPanel();

    private slots:


    private:
        void BuildUserInterface();

    public:
        QString Title() const { return this->m_title; }
        QString FilePath() const { return this->m_filePath; }
        bool IsNewFile() const { return this->m_isNewFile; }
        QUuid SessionUuid() const { return this->m_sessionUuid; }
        QsciScintilla& ScintillaEditor() const { return *this->m_scintilla; }
        QListView& IssuesListView() const { return *this->m_issuesListView; }
        QMemoryVisualizer& MemoryVisualizer() const { return *this->m_memoryVisualizer; }

    // Business logic
    public:
        void SetTitle(QString title);
        void SetFilePath(QString filePath);

    // Signals emitted by the QScintillaPanel
    signals:
        void evTitleUpdated(QString newTitle);
        void evFilePathUpdated(QString newFilePath);
        void evContentUpdated();

    // UI related fields
    private:
        QHBoxLayout* m_centralWidgetLayout;
        QSplitter* m_centralWidgetVSplitter;
        QWidget* m_topHalfWidget;
        QHBoxLayout* m_topHalfWidgetLayout;
        QSplitter* m_topHalfWidgetHSplitter;
        QWidget* m_bottomtHalfWidget;
        QHBoxLayout* m_bottomHalfWidgetLayout;

        QMemoryVisualizer* m_memoryVisualizer;
        QsciScintilla* m_scintilla;
        QTabWidget* m_contentTabRegion;
        QListView* m_issuesListView;
        VisualLinkerScript::Widgets::QLinkerScriptPanel *ui;
    };
};
#endif // MAINWINDOW_H
