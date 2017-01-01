#ifndef TELEPHONEINFODIALOG_H
#define TELEPHONEINFODIALOG_H

#include <QDialog>
#include "telephone.h"
#include "telephonegroup.h"
#include "telephonegroups.h"

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

private:
    Telephone *telephone;
    TelephoneGroups *telephonegroups;
    Ui::TelephoneInfoDialog *ui;
    void displayInfo();
};

#endif // TELEPHONEINFODIALOG_H
