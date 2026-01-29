#ifndef QSEARCHPOPUP_H__
#define QSEARCHPOPUP_H__

#include <QtWidgets>
#include <QString>
#include "Components/QSearchPopup/SearchReplaceRequestTypeEnum.h"
#include "Components/QSearchPopup/SearchPerimeterTypeEnum.h"

class QFlatCheckBox;
class QManualSizeGrip;

/// @brief QSeachPopup
class QSearchPopup : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool NoMatchDetected READ NoMatchDetected USER true)
    Q_PROPERTY(bool MatchDetected READ MatchDetected USER true)
    Q_PROPERTY(bool HasFocus READ HasFocus NOTIFY evFocusChanged)
    
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
    QManualSizeGrip* m_sizeGrip;

    bool m_replaceMode;

    void PopulateDynamicFields();
    void PopulateStaticFields() const;
    void PopulateSearchHistory(const QSettings& settings) const;
    void PopulateReplaceHistory(const QSettings& settings) const;
    void RegisterNewSearchEntry(QSettings& settings, QString newEntry);
    void RegisterNewReplaceEntry(QSettings& settings, QString newEntry);
    
    bool m_noMatchDetected = false;
    bool m_matchDetected = false;
    bool m_hasFocus = false;
    QPointF m_globalPosAtResizeStart;
    QRectF m_rectAtResizeStart;

    SearchReplaceRequestType m_currentSearchRequestType = SearchReplaceRequestType::FindNext;    

public:
	/// @brief Default constructor
	QSearchPopup(QWidget* parent = 0)
		: QFrame(parent),
		  m_replaceMode(false)
	{        
		this->BuildUserInterface();
	}

	/// @brief Default destructor
	~QSearchPopup()
	{}
        
    /// @brief Sets the default text shown when popup is opened.
    void SetText(const QString& defaultText) const;


    /// @brief shows the popup.
    void ShowPopup(bool textBlockSelection = false);


    /// @brief Reports back if no match was detected for the provided text value.
    bool NoMatchDetected() 
    {
        return this->m_noMatchDetected;
    }

	/// @brief Reports back if a was detected for the provided text value.
	bool MatchDetected()
	{
        return this->m_matchDetected;
	}

    /// @brief Reports back if the component as a whole has focus.
    bool HasFocus()
    {
        return this->m_replaceFieldComboBox->hasFocus() ||
               this->m_searchFieldComboBox->hasFocus() ||
               this->m_searchPerimeterComboBox->hasFocus();
    }

    /// @brief Sets the search texbox to focus with the provides string
    void FocusOnSearch(const std::string& stringToSearchFor) const;

protected:
    void BuildUserInterface();  
    void Repolish();    
    SearchPerimeterType CurrentSearchPerimeter() const;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

signals:
    /// @brief Notifies the QT subsystem that the focus has changed.
    void evFocusChanged();

    /// @brief Notifies the recipients that a search or replace action was requested by the user.
    void evSearchReplaceRequested(
        QString searchText,
        QString replaceWith,
        bool caseMatch,
        bool wordMatch,
        bool regEx,
        SearchReplaceRequestType searchReplaceType,
        SearchPerimeterType searchPerimeter);

public slots:
    void OnTextNotFound();
    void OnTextFound();

private slots:
    void OnSizeGripResized(QPointF globalPosition, int dx, int dy);
    void OnSizeGripResizeStarted(QPointF globalPosition);
    void OnToggleSearchReplace();
    void OnSearchActionSelectorClicked();
    void OnClosePopupPressed();
    void OnSearchActionButtonPressed();
    void OnReplaceNextButtonPressed();
    void OnReplaceAllButtonPressed();
};

#endif // end of QSEARCHPOPUP_H__
