#include "persongroupsdialog.h"
#include "ui_persongroupsdialog.h"

PersonGroupsDialog::PersonGroupsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonGroupsDialog)
{
    ui->setupUi(this);
}

PersonGroupsDialog::~PersonGroupsDialog()
{
    delete ui;
}
