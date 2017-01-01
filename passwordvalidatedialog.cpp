#include "passwordvalidatedialog.h"
#include "ui_passwordvalidatedialog.h"

PasswordValidateDialog::PasswordValidateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordValidateDialog)
{
    ui->setupUi(this);
    this->setFixedSize(sizeHint());
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    setWindowFlags(windowFlags()&~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint);
}

PasswordValidateDialog::~PasswordValidateDialog()
{
    delete ui;
}

void PasswordValidateDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == (QPushButton*)button)
        emit this->onValidatePassword(ui->passwordLineEdit->text().toStdString(), this);
}
