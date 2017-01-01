#include "emailgroupinfo.h"
#include "ui_emailgroupinfo.h"

EmailGroupInfo::EmailGroupInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailGroupInfo)
{
    ui->setupUi(this);
}

EmailGroupInfo::~EmailGroupInfo()
{
    delete ui;
}
