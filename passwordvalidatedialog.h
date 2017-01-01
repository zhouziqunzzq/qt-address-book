#ifndef PASSWORDVALIDATEDIALOG_H
#define PASSWORDVALIDATEDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class PasswordValidateDialog;
}

class PasswordValidateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordValidateDialog(QWidget *parent = 0);
    ~PasswordValidateDialog();

private:
    Ui::PasswordValidateDialog *ui;
signals:
    void onValidatePassword(std::string pwd, PasswordValidateDialog* pd);
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // PASSWORDVALIDATEDIALOG_H
