#ifndef PERSONGROUPINFO_H
#define PERSONGROUPINFO_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "persongroup.h"
#include "persongroups.h"

namespace Ui {
class PersonGroupInfo;
}

class PersonGroupInfo : public QDialog
{
    Q_OBJECT

public:
    explicit PersonGroupInfo(PersonGroups *pg, QWidget *parent = 0);
    explicit PersonGroupInfo(PersonGroup *p, PersonGroups *pg, QWidget *parent = 0);
    ~PersonGroupInfo();

protected:
    void closeEvent(QCloseEvent *e);

signals:
    void addNewPersonGroup(PersonGroup *pg);

private slots:
    void displayInfo();
    void saveInfo();

    void on_editPushButton_clicked();

    void on_okPushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::PersonGroupInfo *ui;
    PersonGroups *persongroups;
    PersonGroup *persongroup;
    bool isNew; //是否新增分组
    void disableEdit();
    void enableEdit();
};

#endif // PERSONGROUPINFO_H
