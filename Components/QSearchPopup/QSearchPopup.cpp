#include "QSearchPopup.h"

using namespace VisualLinkerScript::Components;

void QSearchPopup::BuildUserInterface()
{
    this->m_closePopupButton = new QPushButton(this);
    this->m_findOrReplaceSelector = new QPushButton(this);

    this->m_searchTextContainer = new QWidget(this);
    this->m_searchTextEditBox = new QTextEdit(this);
    this->m_searchTextDropdownButton = new QPushButton(this);
    this->m_searchTextRecentEntriesMenu = new QMenu(this);
    this->m_row0HBox = new QHBoxLayout(this);
    this->m_row0HBoxContainer = new QWidget(this);

    this->m_performSearchContainer = new QWidget(this);
    this->m_performSearchActionButton = new QPushButton(this);
    this->m_performSearchDropdownButton = new QPushButton(this);
    this->m_performSearchActionListMenu = new QMenu(this);

    this->m_replaceTextContainer = new QWidget(this);
    this->m_replaceTextEdit = new QTextEdit(this);
    this->m_replaceTextActionSelector = new QPushButton(this);
    this->m_replaceTextActionMenu = new QMenu(this);
    this->m_row1HBox = new QHBoxLayout(this);
    this->m_row1HBoxContainer = new QWidget(this);

    this->m_replaceNextButton = new QPushButton(this);
    this->m_replaceAllButton = new QPushButton(this);

    this->m_matchCaseCheckBox = new QCheckBox(this);
    this->m_matchWordCheckBox = new QCheckBox(this);
    this->m_regExCheckBox = new QCheckBox(this);
    this->m_row2HBox = new QHBoxLayout(this);
    this->m_row2HBoxContainer = new QWidget(this);

    this->m_sizeGrip = new QSizeGrip(this);
    this->m_masterLayout = new QGridLayout(this);

    this->m_masterLayout->addWidget(this->m_findOrReplaceSelector, 0, 0, Qt::AlignTop);
    this->m_masterLayout->addWidget(this->m_sizeGrip, 2, 0, Qt::AlignBottom);
    this->m_masterLayout->addWidget(this->m_row0HBox, 0, 1, Qt::AlignLeading);
    this->m_masterLayout->addWidget(this->m_closePopupButton, 2, 0, Qt::AlignTop);
    this->m_masterLayout->addWidget(this->m_row1HBox, 1, 1, 0, 2, Qt::AlignCenter);
    this->m_masterLayout->addWidget
}


/// @param Sets the default text shown when popup is opened.
void QSearchPopup::SetText(QString defaultText)
{


}



void QSearchPopup::SearchReaplceRequested(
    QString searchText,
    QString replaceWith,
    bool caseMatch,
    bool wordMatch,
    bool regEx,
    SearchRequestType searchType,
    ReplaceRequestType replaceType,
    SearchPerimeterType searchPerimeter)
{

}

