#include "QSearchPopup.h"
#include "../QFlatCheckBox/QFlatCheckBox.h"

SearchPerimeterType ConvertIndexToSearchPerimeter(int comboBoxIndex);

void QSearchPopup::BuildUserInterface()
{
	this->m_row0HBox = new QHBoxLayout();
	this->m_row1HBox = new QHBoxLayout();
	this->m_row2HBox = new QHBoxLayout();
	this->m_vbox = new QVBoxLayout();

	this->m_closePopupButton = new QPushButton(this);
	this->m_closePopupButton->setFixedSize(24, 24);
	this->m_closePopupButton->setFocusPolicy(Qt::NoFocus);
	this->m_closePopupButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_closePopupButton->setIcon(QIcon(":/resources/Images/close-icon.svg").pixmap(QSize(20, 22)));
	QObject::connect(this->m_closePopupButton, &QPushButton::clicked, this, &QSearchPopup::OnClosePopupPressed);
	
	this->m_findOrReplaceSelectorButton = new QPushButton(this);
	this->m_findOrReplaceSelectorButton->setFixedSize(24, 24);
	this->m_findOrReplaceSelectorButton->setFocusPolicy(Qt::NoFocus);
	this->m_findOrReplaceSelectorButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_findOrReplaceSelectorButton->setIcon(QIcon(":/resources/Images/arrow-drop-down-icon.svg").pixmap(QSize(20, 22)));
	QObject::connect(this->m_findOrReplaceSelectorButton, &QPushButton::clicked, this, &QSearchPopup::OnToggleSearchReplace);

	this->m_searchActionButton = new QPushButton(this);
	this->m_searchActionButton->setFixedSize(32, 24);
	this->m_searchActionButton->setToolTip("Find next");
	this->m_searchActionButton->setFocusPolicy(Qt::NoFocus);
	this->m_searchActionButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_searchActionButton->setIcon(QIcon(":/resources/Images/arrow-right-icon.svg").pixmap(QSize(20, 22)));
	QObject::connect(this->m_searchActionButton, &QPushButton::clicked, this, &QSearchPopup::OnSearchActionButtonPressed);

	this->m_searchActionSelectorButton = new QPushButton(this);
	this->m_searchActionSelectorButton->setFixedSize(20, 24);	
	this->m_searchActionSelectorButton->setFocusPolicy(Qt::NoFocus);
	this->m_searchActionSelectorButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_searchActionSelectorButton->setFlat(true);
	this->m_searchActionSelectorButton->setIcon(QIcon(":/resources/Images/expand-more-icon.svg").pixmap(QSize(16, 22)));
	QObject::connect(this->m_searchActionSelectorButton, &QPushButton::clicked, this, &QSearchPopup::OnSearchActionSelectorClicked);

	this->m_replaceNextButton = new QPushButton(this);
	this->m_replaceNextButton->setToolTip("Replace next");
	this->m_replaceNextButton->setFixedSize(24, 24);
	this->m_replaceNextButton->setFocusPolicy(Qt::NoFocus);
	this->m_replaceNextButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_replaceNextButton->setIcon(QIcon(":/resources/Images/find-replace-icon.svg").pixmap(QSize(20, 22)));
	QObject::connect(this->m_replaceNextButton, &QPushButton::clicked, this, &QSearchPopup::OnReplaceNextButtonPressed);

	this->m_replaceAllButton = new QPushButton(this);	
	this->m_replaceAllButton->setToolTip("Replace all");
	this->m_replaceAllButton->setFixedSize(24, 24);
	this->m_replaceAllButton->setFocusPolicy(Qt::NoFocus);
	this->m_replaceAllButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_replaceAllButton->setIcon(QIcon(":/resources/Images/find-replace-icon.svg").pixmap(QSize(20, 22)));
	QObject::connect(this->m_replaceAllButton, &QPushButton::clicked, this, &QSearchPopup::OnReplaceAllButtonPressed);

	this->m_matchCaseCheckButton = new QFlatCheckBox(this);
	this->m_matchCaseCheckButton->setToolTip("Match exact case");
	this->m_matchCaseCheckButton->setFixedSize(24, 24);
	this->m_matchCaseCheckButton->setFocusPolicy(Qt::NoFocus);
	this->m_matchCaseCheckButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_matchCaseCheckButton->setIcon(QIcon(":/resources/Images/match-case-icon.svg").pixmap(QSize(20, 22)));	

	this->m_matchWholeWordCheckButton = new QFlatCheckBox(this);
	this->m_matchWholeWordCheckButton->setToolTip("Match whole word");
	this->m_matchWholeWordCheckButton->setFixedSize(24, 24);
	this->m_matchWholeWordCheckButton->setFocusPolicy(Qt::NoFocus);
	this->m_matchWholeWordCheckButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_matchWholeWordCheckButton->setIcon(QIcon(":/resources/Images/match-word-icon.svg").pixmap(QSize(20, 22)));

	this->m_matchRegularExpressionButton = new QFlatCheckBox(this);
	this->m_matchRegularExpressionButton->setToolTip("Match regular expression");
	this->m_matchRegularExpressionButton->setFixedSize(24, 24);
	this->m_matchRegularExpressionButton->setFocusPolicy(Qt::NoFocus);
	this->m_matchRegularExpressionButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_matchRegularExpressionButton->setIcon(QIcon(":/resources/Images/regular-expression-icon.svg").pixmap(QSize(20, 22)));

	this->m_searchFieldComboBox = new QComboBox(this);
	this->m_searchFieldComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->m_searchFieldComboBox->setEditable(true);
	this->m_searchFieldComboBox->setFixedHeight(24);
	this->m_searchFieldComboBox->installEventFilter(this);
	this->m_searchFieldComboBox->setProperty("inError", false);

	this->m_replaceFieldComboBox = new QComboBox(this);
	this->m_replaceFieldComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->m_replaceFieldComboBox->setEditable(true);
	this->m_replaceFieldComboBox->setFixedHeight(24);
	this->m_replaceFieldComboBox->installEventFilter(this);

	this->m_searchPerimeterComboBox = new QComboBox(this);
	this->m_searchPerimeterComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->m_searchPerimeterComboBox->setFixedHeight(24);	
	this->m_searchPerimeterComboBox->installEventFilter(this);

	this->m_sizeGrip = new QSizeGrip(this);
		
	this->m_row0HBox->setSpacing(2);
	this->m_row0HBox->addWidget(this->m_findOrReplaceSelectorButton);
	this->m_row0HBox->addWidget(this->m_searchFieldComboBox);
	this->m_row0HBox->addWidget(this->m_searchActionButton);
	this->m_row0HBox->addSpacing(-2);
	this->m_row0HBox->addWidget(this->m_searchActionSelectorButton);
	this->m_row1HBox->addSpacing(5);
	this->m_row0HBox->addWidget(this->m_closePopupButton);

	this->m_row1HBox->setSpacing(2);
	this->m_row1HBox->addSpacing(21);
	this->m_row1HBox->addWidget(this->m_replaceFieldComboBox);
	this->m_row1HBox->addWidget(this->m_replaceNextButton);
	this->m_row1HBox->addWidget(this->m_replaceAllButton);
	
	this->m_row2HBox->setSpacing(2);
	this->m_row2HBox->addWidget(this->m_sizeGrip, 0, Qt::AlignBottom | Qt::AlignLeft);
	this->m_row2HBox->addSpacing(6);
	this->m_row2HBox->addWidget(this->m_matchCaseCheckButton);
	this->m_row2HBox->addWidget(this->m_matchWholeWordCheckButton);
	this->m_row2HBox->addWidget(this->m_matchRegularExpressionButton);
	this->m_row2HBox->addWidget(this->m_searchPerimeterComboBox);
	this->m_row2HBox->addSpacing(43);

	this->m_vbox->addLayout(this->m_row0HBox);
	this->m_vbox->addSpacing(2);
	this->m_vbox->addLayout(this->m_row1HBox);
	this->m_vbox->addSpacing(2);
	this->m_vbox->addLayout(this->m_row2HBox);
	this->m_vbox->addSpacing(2);
	this->m_vbox->setSpacing(1);
	this->setLayout(this->m_vbox);
	this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	this->setContentsMargins(0, 0, 0, 0);

	// Populate our fields with historical data (if available)
	PopulateDynamicFields();
	PopulateStaticFields();

	// We need to hide the following entires, as we launch in 'Search' mode, and not 
	// in 'Replace' mode.
	this->m_replaceFieldComboBox->setVisible(false);
	this->m_replaceNextButton->setVisible(false);
	this->m_replaceAllButton->setVisible(false);	
}

void QSearchPopup::OnSearchActionSelectorClicked()
{
	QMenu searchActionSelectorMenu(this);
	auto findNextAction = searchActionSelectorMenu.addAction(
		QIcon(":/resources/Images/arrow-right-icon.svg"),
		"Find Next");

	auto findPrevious = searchActionSelectorMenu.addAction(
		QIcon(":/resources/Images/arrow-left-icon.svg"),
		"Find Previous");

	auto findAll = searchActionSelectorMenu.addAction(
		QIcon(":/resources/Images/search-magnifying-glass-icon.svg"),
		"Find All");

	findNextAction->setShortcut(QKeySequence(Qt::Key_F3));
	findPrevious->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F3));

	QObject::connect(findNextAction, &QAction::triggered, [this]() 
		{
			this->m_currentSearchRequestType = SearchReplaceRequestType::FindNext;
			this->OnSearchActionButtonPressed();
		});

	QObject::connect(findPrevious, &QAction::triggered, [this]() 
		{
			this->m_currentSearchRequestType = SearchReplaceRequestType::FindPrevious;
			this->OnSearchActionButtonPressed();
		});

	QObject::connect(findAll, &QAction::triggered, [this]() 
		{ 
			this->m_currentSearchRequestType = SearchReplaceRequestType::FindAll;
			this->OnSearchActionButtonPressed();
		});

	searchActionSelectorMenu.exec(QCursor::pos());
}

SearchPerimeterType ConvertIndexToSearchPerimeter(int comboBoxIndex)
{
	switch (comboBoxIndex)
	{
	case 0: return SearchPerimeterType::CurrentFile;
	case 1: return SearchPerimeterType::AllOpenFiles;
	case 2: return SearchPerimeterType::Selection;
	default:
		throw std::exception("Index out of range for 'comboBoxIndex'");
	}
}

void QSearchPopup::PopulateStaticFields()
{
	this->m_searchPerimeterComboBox->addItem("Current document");
	this->m_searchPerimeterComboBox->addItem("All open documents");
	if (this->m_textBlockSelected)
	{
		this->m_searchPerimeterComboBox->addItem("Selection");
	}

	// Set perimeter to "Selection" if anything is selected
	if (this->m_textBlockSelected)
	{
		this->m_searchPerimeterComboBox->setCurrentIndex(2);
	}
}

void QSearchPopup::PopulateDynamicFields()
{
	QSettings settings("NasserG64", "VisualLinkerScript");
	this->PopulateReplaceHistory(settings);
	this->PopulateSearchHistory(settings);
}

void QSearchPopup::PopulateSearchHistory(const QSettings& settings)
{
	auto listOfEntries = settings.value("searchHistory").toStringList();
	this->m_searchFieldComboBox->clear();
	this->m_searchFieldComboBox->addItems(listOfEntries);

	if (listOfEntries.count() > 0)
	{
		this->m_searchFieldComboBox->setCurrentIndex(0);
	}
}

void QSearchPopup::PopulateReplaceHistory(const QSettings& settings)
{
	auto listOfEntries = settings.value("replaceHistory").toStringList();
	this->m_replaceFieldComboBox->clear();
	this->m_replaceFieldComboBox->addItems(listOfEntries);

	if (listOfEntries.count() > 0)
	{
		this->m_replaceFieldComboBox->setCurrentIndex(0);
	}
}

void QSearchPopup::RegisterNewSearchEntry(QSettings& settings, QString newEntry)
{
	auto listOfEntries = settings.value("searchHistory").toStringList();
	if (listOfEntries.contains(newEntry, Qt::CaseSensitive))
	{
		listOfEntries.removeAll(newEntry);
	}
	listOfEntries.push_front(newEntry);
	settings.setValue("searchHistory", listOfEntries);		
}

void QSearchPopup::RegisterNewReplaceEntry(QSettings& settings, QString newEntry)
{
	auto listOfEntries = settings.value("replaceHistory").toStringList();
	if (listOfEntries.contains(newEntry, Qt::CaseSensitive))
	{
		listOfEntries.removeAll(newEntry);
	}
	listOfEntries.push_front(newEntry);
	settings.setValue("replaceHistory", listOfEntries);
}

void QSearchPopup::OnToggleSearchReplace()
{
	auto valueToSet = !this->m_replaceFieldComboBox->isVisible();
	auto iconToSetPath = valueToSet ? ":/resources/Images/arrow-drop-up-icon.svg" : ":/resources/Images/arrow-drop-down-icon.svg";
	this->m_replaceFieldComboBox->setVisible(valueToSet);
	this->m_replaceNextButton->setVisible(valueToSet);
	this->m_replaceAllButton->setVisible(valueToSet);
	this->m_findOrReplaceSelectorButton->setIcon(QIcon(iconToSetPath).pixmap(QSize(20, 22)));
}

/// @param Sets the default text shown when popup is opened.
void QSearchPopup::SetText(QString defaultText)
{
	auto foundIndex = this->m_searchFieldComboBox->findText(defaultText);
	if (foundIndex != -1)
	{
		this->m_searchFieldComboBox->setCurrentIndex(foundIndex);
	}
	else
	{
		this->m_searchFieldComboBox->setEditText(defaultText);
	}
}

SearchPerimeterType QSearchPopup::CurrentSearchPerimeter()
{
	return ConvertIndexToSearchPerimeter(this->m_searchPerimeterComboBox->currentIndex());
}

void QSearchPopup::OnTextNotFound()
{
	this->m_noMatchDetected = true;
	this->m_matchDetected = false;
	this->m_searchFieldComboBox->setProperty("inError", true);
	this->Repolish();
}

void QSearchPopup::OnTextFound()
{
	this->m_matchDetected = true;
	this->m_noMatchDetected = false;
	this->m_searchFieldComboBox->setProperty("inError", false);
	this->Repolish();
}

void QSearchPopup::OnClosePopupPressed()
{
	this->hide();
}

void QSearchPopup::OnSearchActionButtonPressed()
{
	emit this->evSearchReaplceRequested(
		this->m_searchFieldComboBox->currentText(),
		this->m_replaceFieldComboBox->currentText(),
		this->m_matchCaseCheckButton->IsChecked(),
		this->m_matchWholeWordCheckButton->IsChecked(),
		this->m_matchRegularExpressionButton->IsChecked(),
		this->m_currentSearchRequestType,
		CurrentSearchPerimeter());		
}

void QSearchPopup::OnReplaceNextButtonPressed()
{
	emit this->evSearchReaplceRequested(
		this->m_searchFieldComboBox->currentText(),
		this->m_replaceFieldComboBox->currentText(),
		this->m_matchCaseCheckButton->IsChecked(),
		this->m_matchWholeWordCheckButton->IsChecked(),
		this->m_matchRegularExpressionButton->IsChecked(),
		SearchReplaceRequestType::ReplaceNext,
		CurrentSearchPerimeter());
}

void QSearchPopup::OnReplaceAllButtonPressed()
{
	emit this->evSearchReaplceRequested(
		this->m_searchFieldComboBox->currentText(),
		this->m_replaceFieldComboBox->currentText(),
		this->m_matchCaseCheckButton->IsChecked(),
		this->m_matchWholeWordCheckButton->IsChecked(),
		this->m_matchRegularExpressionButton->IsChecked(),
		SearchReplaceRequestType::ReplaceAll,
		CurrentSearchPerimeter());
}

bool QSearchPopup::eventFilter(QObject* obj, QEvent* event) 
{
	if ((obj == this->m_searchFieldComboBox) || (obj == this->m_replaceFieldComboBox) || (obj == this->m_searchPerimeterComboBox))
	{
		if ((event->type() == QEvent::FocusIn) || (event->type() == QEvent::FocusOut)) 
		{
			emit this->evFocusChanged();
			this->Repolish();
		}
	}
	return QObject::eventFilter(obj, event);
}

void QSearchPopup::Repolish()
{
	style()->unpolish(this);
	style()->polish(this);
}
