#include "telephonegroupsdialog.h"
#include "ui_telephonegroupsdialog.h"

TelephoneGroupsDialog::TelephoneGroupsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelephoneGroupsDialog)
{
    ui->setupUi(this);
}

TelephoneGroupsDialog::~TelephoneGroupsDialog()
{
    delete ui;
}
