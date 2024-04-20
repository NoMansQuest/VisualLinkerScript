#include "QSearchPopup.h"


SearchPerimeterType ConvertIndexToSearchPerimeter(int comboBoxIndex);

void QSearchPopup::BuildUserInterface()
{
	this->m_row0HBox = new QHBoxLayout();
	this->m_row1HBox = new QHBoxLayout();
	this->m_row2HBox = new QHBoxLayout();
	this->m_vbox = new QVBoxLayout();

	this->m_closePopupButton = new QPushButton(this);
	this->m_closePopupButton->setFixedSize(20, 22);
	this->m_closePopupButton->setFocusPolicy(Qt::NoFocus);
	this->m_closePopupButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_closePopupButton->setIcon(QIcon(":/resources/Images/close-icon.svg").pixmap(QSize(20, 22)));
	
	this->m_findOrReplaceSelectorButton = new QPushButton(this);
	this->m_findOrReplaceSelectorButton->setFixedSize(20, 22);
	this->m_findOrReplaceSelectorButton->setFocusPolicy(Qt::NoFocus);
	this->m_findOrReplaceSelectorButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_findOrReplaceSelectorButton->setIcon(QIcon(":/resources/Images/arrow-drop-down-icon.svg").pixmap(QSize(20, 22)));
	connect(this->m_findOrReplaceSelectorButton, &QPushButton::clicked, this, &QSearchPopup::OnToggleSearchReplace);

	this->m_searchActionButton = new QPushButton(this);
	this->m_searchActionButton->setFixedSize(28, 22);
	this->m_searchActionButton->setFocusPolicy(Qt::NoFocus);
	this->m_searchActionButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_searchActionButton->setIcon(QIcon(":/resources/Images/chevron-right-icon.svg").pixmap(QSize(20, 22)));

	this->m_searchActionSelectorButton = new QPushButton(this);
	this->m_searchActionSelectorButton->setFixedSize(16, 22);
	this->m_searchActionSelectorButton->setFocusPolicy(Qt::NoFocus);
	this->m_searchActionSelectorButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_searchActionSelectorButton->setIcon(QIcon(":/resources/Images/expand-more-icon.svg").pixmap(QSize(16, 22)));

	this->m_replaceNextButton = new QPushButton(this);
	this->m_replaceNextButton->setFixedSize(20, 22);
	this->m_replaceNextButton->setFocusPolicy(Qt::NoFocus);
	this->m_replaceNextButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_replaceNextButton->setIcon(QIcon(":/resources/Images/find-replace-icon.svg").pixmap(QSize(20, 22)));

	this->m_replaceAllButton = new QPushButton(this);	
	this->m_replaceAllButton->setFixedSize(20, 22);
	this->m_replaceAllButton->setFocusPolicy(Qt::NoFocus);
	this->m_replaceAllButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_replaceAllButton->setIcon(QIcon(":/resources/Images/find-replace-icon.svg").pixmap(QSize(20, 22)));

	this->m_matchCaseCheckButton = new QPushButton(this);
	this->m_matchCaseCheckButton->setFixedSize(20, 22);
	this->m_matchCaseCheckButton->setFocusPolicy(Qt::NoFocus);
	this->m_matchCaseCheckButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_matchCaseCheckButton->setIcon(QIcon(":/resources/Images/match-case-icon.svg").pixmap(QSize(20, 22)));

	this->m_matchWholeWordCheckButton = new QPushButton(this);
	this->m_matchWholeWordCheckButton->setFixedSize(20, 22);
	this->m_matchWholeWordCheckButton->setFocusPolicy(Qt::NoFocus);
	this->m_matchWholeWordCheckButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_matchWholeWordCheckButton->setIcon(QIcon(":/resources/Images/match-word-icon.svg").pixmap(QSize(20, 22)));

	this->m_matchRegularExpressionButton = new QPushButton(this);
	this->m_matchRegularExpressionButton->setFixedSize(20, 22);
	this->m_matchRegularExpressionButton->setFocusPolicy(Qt::NoFocus);
	this->m_matchRegularExpressionButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->m_matchRegularExpressionButton->setIcon(QIcon(":/resources/Images/regular-expression-icon.svg").pixmap(QSize(20, 22)));

	this->m_searchFieldComboBox = new QComboBox(this);
	this->m_searchFieldComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->m_searchFieldComboBox->setEditable(true);
	this->m_replaceFieldComboBox = new QComboBox(this);
	this->m_replaceFieldComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->m_replaceFieldComboBox->setEditable(true);
	this->m_searchPerimeterComboBox = new QComboBox(this);
	this->m_searchPerimeterComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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
	this->m_row1HBox->addSpacing(17);
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

/// @param Sets the default text shown when popup is opened.
void QSearchPopup::SetText(QString defaultText)
{

}

void QSearchPopup::OnTextNotFound()
{

}

void QSearchPopup::OnTextFound()
{

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