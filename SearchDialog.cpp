#include "SearchDialog.h"
#include "ui_SearchDialog.h"
#include <QFocusEvent>

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Popup);
    ui->setupUi(this);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::focusOutEvent(QFocusEvent *event)
{
    close();
    QDialog::focusOutEvent(event);
}

void SearchDialog::changeEvent(QEvent *event)
{
    auto typeOfEvent = event->type();

    if (event->type() == QEvent::WindowDeactivate)
    {
        // Dialog has lost focus and become inactive
        qDebug() << "Dialog has become inactive";
        close();
    }
    QDialog::changeEvent(event);
}
