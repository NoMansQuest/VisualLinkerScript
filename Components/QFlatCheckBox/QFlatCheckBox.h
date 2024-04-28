#ifndef QFLATCHECKBOX_H__
#define QFLATCHECKBOX_H__

#include <QtWidgets>
#include <QString>

/// @brief The QFlatCheckBox is composed of QCheckBox objects
class QFlatCheckBox : public QPushButton
{
	Q_OBJECT
    Q_PROPERTY(bool IsChecked READ IsChecked WRITE SetCheckedState NOTIFY IsCheckedChanged)

private:
	bool m_isChecked = false;

public:
	/// @brief Default constructor
	QFlatCheckBox(QWidget* parent = nullptr)
		: QPushButton(parent)
	{
		QObject::connect(this, &QFlatCheckBox::pressed, this, &QFlatCheckBox::OnPressed);
	}

	QFlatCheckBox(const QString& text, QWidget* parent = nullptr)
		: QPushButton(text, parent)
	{}

	/// @brief Default destructor
	~QFlatCheckBox()
	{}

public:

	/// @brief Reports back the current state of the QFlatCheckBox
	bool IsChecked() const
	{
		return this->m_isChecked;
	}

	/// @brief Forces the check-box into either check or non-checked state
	void SetCheckedState(bool isChecked)
	{
		this->m_isChecked = isChecked;
		this->setDown(isChecked);
		emit this->IsCheckedChanged();
		style()->unpolish(this);
		style()->polish(this);
	}

signals:
	void IsCheckedChanged();
	void CheckStateChanged();

protected slots:

	/// @brief Intercepts QPushButton's 'pressed'
	void OnPressed()
	{
		this->SetCheckedState(!this->m_isChecked);
	}
};

#endif