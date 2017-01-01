#include "telephoneinfodialog.h"
#include "ui_telephoneinfodialog.h"

TelephoneInfoDialog::TelephoneInfoDialog(TelephoneGroups *tg, QWidget *parent) :
    QDialog(parent),
    telephonegroups(tg),
    ui(new Ui::TelephoneInfoDialog)
{
    ui->setupUi(this);
}

TelephoneInfoDialog::TelephoneInfoDialog(Telephone *t, TelephoneGroups *tg, QWidget *parent) :
    QDialog(parent),
    telephone(t),
    telephonegroups(tg),
    ui(new Ui::TelephoneInfoDialog)
{
    ui->setupUi(this);
    //Display
    this->displayInfo();
}

TelephoneInfoDialog::~TelephoneInfoDialog()
{
    delete ui;
}

void TelephoneInfoDialog::displayInfo()
{
    ui->telLineEdit->setText(QString::fromStdString(this->telephone->tel));
    ui->groupLineEdit->setText(QString::fromStdString(this->telephonegroups->findByID(this->telephone->groupID)->data.name));
}
