#ifndef EMAILGROUPINFO_H
#define EMAILGROUPINFO_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include "emailgroup.h"
#include "emailgroups.h"

namespace Ui {
class EmailGroupInfo;
}

class EmailGroupInfo : public QDialog
{
    Q_OBJECT

public:
    //New email group
    explicit EmailGroupInfo(EmailGroups *eg, QWidget *parent = 0);
    //Edit email group
    explicit EmailGroupInfo(EmailGroup *e,  EmailGroups *eg, QWidget *parent = 0);
    ~EmailGroupInfo();

protected:
    void closeEvent(QCloseEvent *e);

signals:
    void addNewEmailGroup(EmailGroup *eg);

private slots:
    void displayInfo();
    void saveInfo();

    void on_editPushButton_clicked();

    void on_okPushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::EmailGroupInfo *ui;
    EmailGroups *emailgroups;
    EmailGroup *emailgroup;
    bool isNew; //是否新增分组
    void disableEdit();
    void enableEdit();
};

#endif // EMAILGROUPINFO_H
