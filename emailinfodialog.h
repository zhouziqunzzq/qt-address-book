#ifndef EMAILINFODIALOG_H
#define EMAILINFODIALOG_H

#include <QDialog>

namespace Ui {
class EmailInfoDialog;
}

class EmailInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmailInfoDialog(QWidget *parent = 0);
    ~EmailInfoDialog();

private:
    Ui::EmailInfoDialog *ui;
};

#endif // EMAILINFODIALOG_H
