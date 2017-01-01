#ifndef TELEPHONEGROUPINFO_H
#define TELEPHONEGROUPINFO_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "telephonegroup.h"
#include "telephonegroups.h"

namespace Ui {
class TelephoneGroupInfo;
}

class TelephoneGroupInfo : public QDialog
{
    Q_OBJECT

public:
    explicit TelephoneGroupInfo(TelephoneGroups *tg, QWidget *parent = 0);
    explicit TelephoneGroupInfo(TelephoneGroup *t, TelephoneGroups *tg, QWidget *parent = 0);
    ~TelephoneGroupInfo();

protected:
    void closeEvent(QCloseEvent *e);

signals:
    void addNewTelephoneGroup(TelephoneGroup *tg);

private slots:
    void displayInfo();
    void saveInfo();

    void on_editPushButton_clicked();

    void on_okPushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::TelephoneGroupInfo *ui;
    TelephoneGroups *telephonegroups;
    TelephoneGroup *telephonegroup;
    bool isNew; //是否新增分组
    void disableEdit();
    void enableEdit();
};


#endif // TELEPHONEGROUPINFO_H
