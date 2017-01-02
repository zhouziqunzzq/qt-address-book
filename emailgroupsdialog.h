#ifndef EMAILGROUPSDIALOG_H
#define EMAILGROUPSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include "emailgroup.h"
#include "emailgroups.h"
#include "emailgroupinfo.h"

namespace Ui {
class EmailGroupsDialog;
}

class EmailGroupsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmailGroupsDialog(EmailGroups *eg, bool isSelection, QWidget *parent = 0);
    ~EmailGroupsDialog();
    enum{
        IDColumn,
        NameColumn
    };

signals:
    void selectEmailGroup(EmailGroup *eg);
    void cleanEmailGroup(EmailGroup *eg);   //设置分组中全部元素为未分组

private:
    Ui::EmailGroupsDialog *ui;
    QStandardItemModel *model;
    EmailGroups *emailgroups;
    void setupModel();
    void setupTableView();

private slots:
    void updateTableView();
    void onAddNewEmailGroup(EmailGroup *eg);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_newPushButton_clicked();
    void on_delPushButton_clicked();
    void on_okPushButton_clicked();
    void on_cancelPushButton_clicked();
};
#endif // EMAILGROUPSDIALOG_H
