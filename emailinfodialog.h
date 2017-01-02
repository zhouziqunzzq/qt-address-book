#ifndef EMAILINFODIALOG_H
#define EMAILINFODIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "email.h"
#include "emailgroup.h"
#include "emailgroups.h"
#include "emailgroupsdialog.h"

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

protected:
    void closeEvent(QCloseEvent *e);

signals:
    void addNewEmail(Email *e);

private slots:
    void displayInfo();
    void saveInfo();

    void on_editPushButton_clicked();

    void on_okPushButton_clicked();

    void on_cancelPushButton_clicked();

    void on_groupSelectPushButton_clicked();

    void onSelectEmailGroup(EmailGroup *eg);

private:
    Ui::EmailInfoDialog *ui;
    Email *email;
    EmailGroups *emailgroups;
    bool isNew; //是否新增
    void disableEdit();
    void enableEdit();
};

#endif // EMAILINFODIALOG_H
