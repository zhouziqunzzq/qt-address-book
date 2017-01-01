#ifndef TELEPHONEGROUPSDIALOG_H
#define TELEPHONEGROUPSDIALOG_H

#include <QDialog>

namespace Ui {
class TelephoneGroupsDialog;
}

class TelephoneGroupsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TelephoneGroupsDialog(QWidget *parent = 0);
    ~TelephoneGroupsDialog();

private:
    Ui::TelephoneGroupsDialog *ui;
};

#endif // TELEPHONEGROUPSDIALOG_H
