#ifndef PERSONGROUPSDIALOG_H
#define PERSONGROUPSDIALOG_H

#include <QDialog>

namespace Ui {
class PersonGroupsDialog;
}

class PersonGroupsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PersonGroupsDialog(QWidget *parent = 0);
    ~PersonGroupsDialog();

private:
    Ui::PersonGroupsDialog *ui;
};

#endif // PERSONGROUPSDIALOG_H
