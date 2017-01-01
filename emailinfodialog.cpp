#include "emailinfodialog.h"
#include "ui_emailinfodialog.h"

EmailInfoDialog::EmailInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailInfoDialog)
{
    ui->setupUi(this);
}

EmailInfoDialog::~EmailInfoDialog()
{
    delete ui;
}
