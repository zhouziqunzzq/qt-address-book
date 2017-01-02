#ifndef PERSONGROUPSDIALOG_H
#define PERSONGROUPSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include "persongroup.h"
#include "persongroups.h"
#include "persongroupinfo.h"

namespace Ui {
class PersonGroupsDialog;
}

class PersonGroupsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PersonGroupsDialog(PersonGroups *pg, bool isSelection, QWidget *parent = 0);
    ~PersonGroupsDialog();
    enum{
        IDColumn,
        NameColumn
    };

signals:
    void selectPersonGroup(PersonGroup *pg);
    void cleanPersonGroup(PersonGroup *pg);   //设置分组中全部元素为未分组
    void clearPersonGroup(PersonGroup *pg);   //清空分组

private:
    Ui::PersonGroupsDialog *ui;
    QStandardItemModel *model;
    PersonGroups *persongroups;
    void setupModel();
    void setupTableView();

private slots:
    void updateTableView();
    void onAddNewPersonGroup(PersonGroup *pg);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_newPushButton_clicked();
    void on_delPushButton_clicked();
    void on_okPushButton_clicked();
    void on_cancelPushButton_clicked();
};

#endif // PERSONGROUPSDIALOG_H
