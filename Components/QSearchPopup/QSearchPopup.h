#ifndef QSEARCHPOPUP_H__
#define QSEARCHPOPUP_H__

#include <QtWidgets>
#include <QString>
#include "SearchRequestTypeEnum.h"
#include "ReplaceRequestTypeEnum.h"
#include "SearchPerimeterTypeEnum.h"

class QFlatCheckBox;

/// @brief QSeachPopup
class QSearchPopup : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool NoMatchDetected READ NoMatchDetected USER true)
    Q_PROPERTY(bool HasFocus READ HasFocus NOTIFY HasFocusChanged)
    Q_PROPERTY(SearchRequestType CurrentSearchRequestType READ CurrentSearchRequestType USER true)
    
private:
    QHBoxLayout* m_row0HBox;
    QHBoxLayout* m_row1HBox;
    QHBoxLayout* m_row2HBox;
    QHBoxLayout* m_row3HBox;
    QVBoxLayout* m_vbox;

    QPushButton* m_closePopupButton;
    QPushButton* m_findOrReplaceSelectorButton;
    QPushButton* m_searchActionButton;
    QPushButton* m_searchActionSelectorButton;
    QComboBox* m_searchFieldComboBox;
    QComboBox* m_replaceFieldComboBox;
    QPushButton* m_replaceNextButton;
    QPushButton* m_replaceAllButton;
    QFlatCheckBox* m_matchCaseCheckButton;
    QFlatCheckBox* m_matchWholeWordCheckButton;
    QFlatCheckBox* m_matchRegularExpressionButton;
    QComboBox* m_searchPerimeterComboBox;
    QSizeGrip* m_sizeGrip;

    void PopulateDynamicFields();
    void PopulateStaticFields();
    void PopulateSearchHistory(const QSettings& settings);
    void PopulateReplaceHistory(const QSettings& settings);
    void RegisterNewSearchEntry(QSettings& settings, QString newEntry);
    void RegisterNewReplaceEntry(QSettings& settings, QString newEntry);

    bool m_textBlockSelected = false;
    bool m_noMatchDetected = false;
    bool m_hasFocus = false;

    SearchRequestType m_currentSearchRequestType = SearchRequestType::FindNext;

public:
	/// @brief Default constructor
	QSearchPopup(bool textBlockSelected, QWidget* parent = 0)
		: QFrame(parent)
	{
        this->m_textBlockSelected = textBlockSelected;
		this->BuildUserInterface();
	}

	/// @brief Default destructor
	~QSearchPopup()
	{}
        
    /// @brief Sets the default text shown when popup is opened.
    void SetText(QString defaultText);


    /// @brief Reports back if no match was detected for the provided text value.
    bool NoMatchDetected() 
    {
        return this->m_noMatchDetected;
    }

    /// @brief Reports back the type of search currently set by the user
    SearchRequestType CurrentSearchRequestType()
    {
        return this->m_currentSearchRequestType;
    }

    /// @brief Reports back if the component as a whole has focus.
    bool HasFocus()
    {
        return this->m_replaceFieldComboBox->hasFocus() ||
            this->m_searchFieldComboBox->hasFocus() ||
            this->m_searchPerimeterComboBox->hasFocus();
    }

protected:
    void BuildUserInterface();  
    void TriggerSearchRequest();

signals:
    /// @brief Notifies the QT subsystem that the focus has changed.
    void HasFocusChanged();

    /// @brief Notifies the recepients that a search or replace action was requested by the user.
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
    void OnTextFound();
    void OnToggleSearchReplace();
    void OnSearchActionSelectorClicked();
};

#endif // end of QSEARCHPOPUP_H__