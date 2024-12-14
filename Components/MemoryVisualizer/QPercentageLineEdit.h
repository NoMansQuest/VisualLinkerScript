#ifndef QLINE_EDIT_PERCENTAGE_H__
#define QLINE_EDIT_PERCENTAGE_H__

#include <QApplication>
#include <QLineEdit>
#include <QIntValidator>
#include <QVBoxLayout>
#include <QWidget>

/// @brief QLineEdit supporting percentage
class QPercentageLineEdit final : public QLineEdit
{
    Q_OBJECT
	Q_PROPERTY(int Percentage READ Percentage WRITE SetPercentage NOTIFY PercentageChanged)
	int m_percentage;

public:

    /// @brief Default constructor
    QPercentageLineEdit(QWidget* parent = nullptr)
        : QLineEdit(parent)
	{        
        auto validator = new QIntValidator(1, 100, this);
        setValidator(validator);
        connect(this, &QLineEdit::textChanged, this, &QPercentageLineEdit::OnTextChanged);
        connect(this, &QLineEdit::editingFinished, this, &QPercentageLineEdit::OnEditingFinished);
        this->setText("100%");
    }

    [[nodiscard]] int Percentage() const
	{
        return m_percentage;
    }

    void SetPercentage(const int percentage)
	{
        if (m_percentage != percentage) 
        {
            m_percentage = qBound(0, percentage, 100);
            this->UpdateText();
            emit PercentageChanged(m_percentage);
        }
    }

private:    
    void UpdateText()
	{
        this->setText(QString::number(m_percentage) + "%");
    }

signals:
    void PercentageChanged(int newPercentage);

private slots:

    void OnTextChanged(const QString& text)
	{
        if (!text.endsWith('%')) 
        {
            QString cleanText = text;
            cleanText.remove('%');
            setText(cleanText + "%");
        }

        bool ok;
        int value = text.left(text.size() - 1).toInt(&ok);
        if (ok && value != m_percentage) 
        {
            m_percentage = qBound(0, value, 100);
            emit PercentageChanged(m_percentage);
        }
    }

    void OnEditingFinished()
	{
        QString cleanText = text();
        cleanText.remove('%'); 
        int value = cleanText.toInt();
        value = qBound(0, value, 100);
        SetPercentage(value);
    }
};

#endif