#ifndef PERSONINFODIALOG_H
#define PERSONINFODIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QTableView>
#include <QStandardItemModel>
#include <vector>
#include <QCloseEvent>
#include <algorithm>
#include "persons.h"
#include "persongroup.h"
#include "persongroups.h"
#include "telephone.h"
#include "telephonegroup.h"
#include "telephonegroups.h"
#include "email.h"
#include "emailgroup.h"
#include "emailgroups.h"
#include "telephoneinfodialog.h"
#include "emailinfodialog.h"
#include "persongroupsdialog.h"

namespace Ui {
class PersonInfoDialog;
}

class PersonInfoDialog : public QDialog
{
    Q_OBJECT

public:
    //New person
    explicit PersonInfoDialog(PersonGroups *pg, TelephoneGroups *tg, EmailGroups *eg, QWidget *parent = 0);
    //Edit person
    explicit PersonInfoDialog(Person *person, PersonGroups *pg, TelephoneGroups *tg, EmailGroups *eg, QWidget *parent);
    ~PersonInfoDialog();
    enum{
        IDColumn,
        TelColumn,
        GroupColumn
    };

protected:
    void closeEvent(QCloseEvent *e);

signals:
    void addNewPerson(Person *p);
    void needUpdatePersonsView();

private slots:
    void on_morePushButton_clicked();
    void on_telTableView_doubleClicked(const QModelIndex &index);
    void on_emailTableView_doubleClicked(const QModelIndex &index);
    void on_cancelPushButton_clicked();
    void on_editPushButton_clicked();
    void on_OkPushButton_clicked();
    void updateTelTableView();
    void updateEmailTableView();
    void on_addTelPushButton_clicked();
    void onAddNewTel(Telephone *t);
    void onAddNewEmail(Email *e);
    void on_delTelPushButton_clicked();
    void on_addEmailPushButton_clicked();
    void on_delEmailPushButton_clicked();
    void on_changeGroupPushButton_clicked();
    void onSelectPersonGroup(PersonGroup *pg);

private:
    Ui::PersonInfoDialog *ui;
    Person *p;
    Person *tempPerson;
    PersonGroups *persongroups;
    TelephoneGroups *telephonegroups;
    EmailGroups *emailgroups;
    bool isHidden;
    bool isNew; //是否是新增联系人
    void displayInfo();
    void saveInfo();
    void disableEdit();
    void enableEdit();
    QStandardItemModel *telModel;
    void setupTelModel();
    void setupTelTableView();
    QStandardItemModel *emailModel;
    void setupEmailModel();
    void setupEmailTableView();

};

#endif // PERSONINFODIALOG_H
