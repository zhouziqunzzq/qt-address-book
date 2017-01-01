#include "emailgroupsdialog.h"
#include "ui_emailgroupsdialog.h"

EmailGroupsDialog::EmailGroupsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailGroupsDialog)
{
    ui->setupUi(this);
}

EmailGroupsDialog::~EmailGroupsDialog()
{
    delete ui;
}
