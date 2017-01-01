#include "telephonegroupinfo.h"
#include "ui_telephonegroupinfo.h"

TelephoneGroupInfo::TelephoneGroupInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelephoneGroupInfo)
{
    ui->setupUi(this);
}

TelephoneGroupInfo::~TelephoneGroupInfo()
{
    delete ui;
}
