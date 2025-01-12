#include "QJumpToLineDialog.h"
#include "LinkerScriptManager/QLinkerScriptManager.h"

void QJumpToLineDialog::BuildInterface()
{
    this->m_intValidator = new QIntValidator(1, 10000, this);

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);    
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 0, 2, 0);
    layout->setSpacing(0);

    auto label = new QLabel(this);
    label->setText("Jump to line: ");
    label->setStyleSheet("QLabel { color: #ffa0a0a0; background: transparent; }");
    label->setContentsMargins(5, 0, 5, 0);

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setText("1"); // This is to prevent IntValidator from complaining at Init state.
    m_lineEdit->setValidator(this->m_intValidator);
    m_lineEdit->setFixedWidth(180);
    m_lineEdit->setFixedHeight(23);    
    m_lineEdit->setContentsMargins(0, 1, 0, 1);
    m_lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_lineEdit->installEventFilter(this);

    layout->addWidget(label);
    layout->addWidget(m_lineEdit);
    QObject::connect(m_lineEdit, &QLineEdit::returnPressed, this, &QJumpToLineDialog::onLineEditReturnPressed);    
    setLayout(layout);
    adjustSize();
}

void QJumpToLineDialog::onLineEditReturnPressed()
{	
    emit this->evNavigateToLine(m_lineEdit->text().toInt());
    accept();
}

void QJumpToLineDialog::setFocusToTextEdit() const
{
    this->m_lineEdit->setFocus(Qt::FocusReason::OtherFocusReason);
}

void QJumpToLineDialog::setRange(
    const uint32_t max, 
    const uint32_t currentLine) const
{
    this->m_lineEdit->setValidator(nullptr);
    this->m_intValidator->setRange(1, max);
    this->m_lineEdit->setText(QString::number(currentLine));
    this->m_lineEdit->setValidator(this->m_intValidator);
}

bool QJumpToLineDialog::eventFilter(QObject* obj, QEvent* event) 
{
    if (event->type() == QEvent::FocusOut)
    {
        this->hide();
        return true;
    }

    return false;
}

void QJumpToLineDialog::focusOutEvent(QFocusEvent* event)
{
    this->hide();
    QDialog::focusOutEvent(event);
}

void QJumpToLineDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event); 
    this->m_lineEdit->setFocus();    
}
