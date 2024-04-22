#ifndef QSEARCHPOPUP_H__
#define QSEARCHPOPUP_H__

#include <QtWidgets>
#include <QString>
#include "SearchRequestTypeEnum.h"
#include "ReplaceRequestTypeEnum.h"
#include "SearchPerimeterTypeEnum.h"

/// @brief QSeachPopup
class QSearchPopup : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool NoMatchDetected READ NoMatchDetected USER true)
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
    QPushButton* m_matchCaseCheckButton;
    QPushButton* m_matchWholeWordCheckButton;
    QPushButton* m_matchRegularExpressionButton;
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

    SearchRequestType m_currentSearchRequestType = SearchRequestType::FindNext;

public:
	/// @brief Default constructor
	QSearchPopup(bool textBlockSelected, QWidget* parent = 0)
		: QWidget(parent)
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

protected:
    void BuildUserInterface();  
    void TriggerSearchRequest();

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
    void OnTextFound();
    void OnToggleSearchReplace();
    void OnSearchActionSelectorClicked();
};

#endif // end of QSEARCHPOPUP_H__