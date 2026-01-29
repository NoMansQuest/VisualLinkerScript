#ifndef QJUMP_TO_LINE_H__
#define QJUMP_TO_LINE_H__

#include <QApplication>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QKeyEvent>
#include <QDebug>


class QJumpToLineDialog : public QDialog
{
    Q_OBJECT

public:
    /// @brief Default constructor
    explicit QJumpToLineDialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        this->BuildInterface();
    }

    /// @brief Sets the focus to the line editor.
    void setFocusToTextEdit() const;

    /// @brief Update line range and current line we're on.
    void setRange(uint32_t max, uint32_t currentLine) const;

protected:
    void focusOutEvent(QFocusEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void showEvent(QShowEvent* event) override;

signals:
    void evNavigateToLine(uint32_t lineNumber);

private slots:
    void BuildInterface();
    void onLineEditReturnPressed();

private:
    QLineEdit* m_lineEdit;
    QIntValidator* m_intValidator;
};


#endif
