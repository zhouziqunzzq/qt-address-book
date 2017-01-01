#include "emailinfodialog.h"
#include "ui_emailinfodialog.h"

EmailInfoDialog::EmailInfoDialog(EmailGroups *eg, QWidget *parent) :
    QDialog(parent),
    emailgroups(eg),
    ui(new Ui::EmailInfoDialog)
{
    ui->setupUi(this);
}

EmailInfoDialog::EmailInfoDialog(Email *e, EmailGroups *eg, QWidget *parent) :
    QDialog(parent),
    emailgroups(eg),
    email(e),
    ui(new Ui::EmailInfoDialog)
{
    ui->setupUi(this);
    //Display info
    this->displayInfo();
}

EmailInfoDialog::~EmailInfoDialog()
{
    delete ui;
}

void EmailInfoDialog::displayInfo()
{
    ui->emailLineEdit->setText(QString::fromStdString(this->email->email));
    ui->groupLineEdit->setText(QString::fromStdString(this->emailgroups->findByID(this->email->groupID)->data.name));
}
