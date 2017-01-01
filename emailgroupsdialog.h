#ifndef EMAILGROUPSDIALOG_H
#define EMAILGROUPSDIALOG_H

#include <QDialog>

namespace Ui {
class EmailGroupsDialog;
}

class EmailGroupsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmailGroupsDialog(QWidget *parent = 0);
    ~EmailGroupsDialog();

private:
    Ui::EmailGroupsDialog *ui;
};

#endif // EMAILGROUPSDIALOG_H
