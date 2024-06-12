#ifndef QLISTASSISTEDINPUT_H__
#define QLISTASSISTEDINPUT_H__

#include <QtWidgets>
#include <QString>
#include <QTextEdit>
#include <vector>

/// @brief QLabelAssistedInput
class QLabelAssistedInput : public QFrame
{
	Q_OBJECT

private:
	QVBoxLayout* m_masterVBox;
	QTextEdit* m_textEdit;
	QLabel* m_label;
	bool m_isInError;

public:
	/// @brief Default constructor
	QLabelAssistedInput(bool textBlockSelected, QWidget* parent = 0)
		: QFrame(parent)
	{		
		this->BuildUserInterface();
	}

	/// @brief Default destructor
	~QLabelAssistedInput()
	{}

private:
	void BuildUserInterface();

signals:
	/// @brief Notifies the QT subsystem that the focus has changed.
	void evInputValidated(QString validatedInput);
};

#endif // end of QLISTASSISTEDINPUT_H__