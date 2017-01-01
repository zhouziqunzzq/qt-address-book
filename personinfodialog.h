#ifndef PERSONINFODIALOG_H
#define PERSONINFODIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QTableView>
#include <QStandardItemModel>
#include <vector>
#include "persons.h"
#include "persongroup.h"
#include "persongroups.h"
#include "telephone.h"
#include "telephonegroup.h"
#include "telephonegroups.h"
#include "email.h"
#include "emailgroup.h"
#include "emailgroups.h"

namespace Ui {
class PersonInfoDialog;
}

class PersonInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PersonInfoDialog(TelephoneGroups *tg, EmailGroups *eg, QWidget *parent = 0);
    explicit PersonInfoDialog(Person *person, TelephoneGroups *tg, EmailGroups *eg, QWidget *parent);
    ~PersonInfoDialog();
    enum{
        IDColumn,
        TelColumn,
        GroupColumn
    };
private slots:
    void on_morePushButton_clicked();

private:
    Ui::PersonInfoDialog *ui;
    Person *p;
    TelephoneGroups *telephonegroups;
    EmailGroups *emailgroups;
    bool isHidden;
    bool isNew; //标记是否是新建联系人
    void displayPersonInfo();
    void disableEdit();
    QStandardItemModel *telModel;
    void setupTelModel();
    void setupTelTableView();
    void updateTelTableView();
    QStandardItemModel *emailModel;
    void setupEmailModel();
    void setupEmailTableView();
    void updateEmailTableView();
};

#endif // PERSONINFODIALOG_H
