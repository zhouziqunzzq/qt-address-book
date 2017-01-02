#ifndef TELEPHONEINFODIALOG_H
#define TELEPHONEINFODIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "telephone.h"
#include "telephonegroup.h"
#include "telephonegroups.h"
#include "telephonegroupsdialog.h"

namespace Ui {
class TelephoneInfoDialog;
}

class TelephoneInfoDialog : public QDialog
{
    Q_OBJECT

public:
    //New telephone
    explicit TelephoneInfoDialog(TelephoneGroups *tg, QWidget *parent = 0);
    //Edit telephone
    explicit TelephoneInfoDialog(Telephone *t, TelephoneGroups *tg, QWidget *parent = 0);
    ~TelephoneInfoDialog();

protected:
    void closeEvent(QCloseEvent *e);

signals:
    void addNewTelephone(Telephone *t);

private slots:
    void displayInfo();
    void saveInfo();

    void on_editPushButton_clicked();

    void on_okPushButton_clicked();

    void on_cancelPushButton_clicked();

    void on_groupSelectPushButton_clicked();

    void onSelectTelephoneGroup(TelephoneGroup *tg);

private:
    Telephone *telephone;
    TelephoneGroups *telephonegroups;
    Ui::TelephoneInfoDialog *ui;
    bool isNew; //是否新增
    void disableEdit();
    void enableEdit();
};

#endif // TELEPHONEINFODIALOG_H
