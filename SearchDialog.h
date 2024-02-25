#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QEvent>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

protected:
    void focusOutEvent(QFocusEvent *event) override;
    void changeEvent(QEvent* event) override;

private:
    Ui::SearchDialog *ui;
};

#endif // SEARCHDIALOG_H
