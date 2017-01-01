#ifndef EMAILINFODIALOG_H
#define EMAILINFODIALOG_H

#include <QDialog>
#include "email.h"
#include "emailgroup.h"
#include "emailgroups.h"

namespace Ui {
class EmailInfoDialog;
}

class EmailInfoDialog : public QDialog
{
    Q_OBJECT

public:
    //New email
    explicit EmailInfoDialog(EmailGroups *eg, QWidget *parent = 0);
    //Edit email
    explicit EmailInfoDialog(Email *e, EmailGroups *eg, QWidget *parent = 0);
    ~EmailInfoDialog();

private:
    Ui::EmailInfoDialog *ui;
    Email *email;
    EmailGroups *emailgroups;
    void displayInfo();
};

#endif // EMAILINFODIALOG_H
