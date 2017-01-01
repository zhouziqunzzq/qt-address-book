#include "telephoneinfodialog.h"
#include "ui_telephoneinfodialog.h"

TelephoneInfoDialog::TelephoneInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelephoneInfoDialog)
{
    ui->setupUi(this);
}

TelephoneInfoDialog::~TelephoneInfoDialog()
{
    delete ui;
}
