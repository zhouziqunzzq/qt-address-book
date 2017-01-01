#ifndef TELEPHONEINFODIALOG_H
#define TELEPHONEINFODIALOG_H

#include <QDialog>

namespace Ui {
class TelephoneInfoDialog;
}

class TelephoneInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TelephoneInfoDialog(QWidget *parent = 0);
    ~TelephoneInfoDialog();

private:
    Ui::TelephoneInfoDialog *ui;
};

#endif // TELEPHONEINFODIALOG_H
