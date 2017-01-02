#ifndef TELEPHONEGROUPSDIALOG_H
#define TELEPHONEGROUPSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include "telephonegroup.h"
#include "telephonegroups.h"
#include "telephonegroupinfo.h"

namespace Ui {
class TelephoneGroupsDialog;
}

class TelephoneGroupsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TelephoneGroupsDialog(TelephoneGroups *tg, bool isSelection, QWidget *parent = 0);
    ~TelephoneGroupsDialog();

    enum{
        IDColumn,
        NameColumn,
        CountColumn
    };

signals:
    void selectTelephoneGroup(TelephoneGroup *tg);
    void cleanTelephoneGroup(TelephoneGroup *tg);   //设置分组中全部元素为未分组

private:
    Ui::TelephoneGroupsDialog *ui;
    QStandardItemModel *model;
    TelephoneGroups *telephonegroups;
    void setupModel();
    void setupTableView();

private slots:
    void updateTableView();
    void onAddNewTelephoneGroup(TelephoneGroup *tg);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_newPushButton_clicked();
    void on_delPushButton_clicked();
    void on_okPushButton_clicked();
    void on_cancelPushButton_clicked();
};

#endif // TELEPHONEGROUPSDIALOG_H
