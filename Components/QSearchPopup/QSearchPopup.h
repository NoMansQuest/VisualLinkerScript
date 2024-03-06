#ifndef QSEARCHPOPUP_H__
#define QSEARCHPOPUP_H__

#include <QtWidgets>
#include <QString>

namespace VisualLinkerScript::Components
{

    /// @brief Search request type
    enum class SearchRequestType
    {
        FindNext,
        FindPrevious,
        FindAll        
    };

    /// @brief Replace request type
    enum class ReplaceRequestType
    {
        ReplaceNext,
        ReplaceAll
    };

    /// @brief Search perimeter
    enum class SearchPerimeterType
    {
        CurrentFile,
        AllOpenFiles
    };


    /// @brief QSeachPopup
    class QSearchPopup : public QWidget
    {
        Q_OBJECT

    public:
        /// @brief Default constructor
        QSearchPopup(QWidget* parent = 0)
            : QWidget(parent)
        {}

        /// @brief Default desctructor
        ~QSearchPopup()
        {}

    private:        
        QPushButton* m_closePopupButton;
        QPushButton* m_findOrReplaceSelector;

        QWidget* m_searchTextContainer;
        QTextEdit* m_searchTextEditBox;
        QPushButton* m_searchTextActioButton;
        QPushButton* m_searchTextDropdownButton;
        QMenu* m_searchTextRecentEntriesMenu;
        QHBoxLayout* m_row0HBox;
        QWidget* m_row0HBoxContainer;

        QWidget* m_performSearchContainer;
        QPushButton* m_performSearchActionButton;
        QPushButton* m_performSearchDropdownButton;
        QMenu* m_performSearchActionListMenu;

        QWidget* m_replaceTextContainer;
        QTextEdit* m_replaceTextEdit;
        QPushButton* m_replaceTextActionSelector;
        QMenu* m_replaceTextActionMenu;
        QHBoxLayout* m_row1HBox;
        QWidget* m_row1HBoxContainer;

        QPushButton* m_replaceNextButton;
        QPushButton* m_replaceAllButton;

        QCheckBox* m_matchCaseCheckBox;
        QCheckBox* m_matchWordCheckBox;
        QCheckBox* m_regExCheckBox;
        QComboBox* m_searchPerimeterSelector;
        QHBoxLayout* m_row2HBox;
        QWidget* m_row2HBoxContainer;

        QSizeGrip* m_sizeGrip;
        QGridLayout* m_masterLayout;
    
    public:
        /// @param Sets the default text shown when popup is opened.
        void SetText(QString defaultText);

    protected:
        void BuildUserInterface();

    signals:
        void SearchReaplceRequested(
            QString searchText,
            QString replaceWith,
            bool caseMatch,
            bool wordMatch,
            bool regEx,
            SearchRequestType searchType,
            ReplaceRequestType replaceType,
            SearchPerimeterType searchPerimeter);

    protected slots:
        void OnTextNotFound();
    };
};


#endif // end of QLINKERSCRIPTSESSION_H__
