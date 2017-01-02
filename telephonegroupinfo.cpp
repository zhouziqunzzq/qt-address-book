#include "telephonegroupinfo.h"
#include "ui_telephonegroupinfo.h"

TelephoneGroupInfo::TelephoneGroupInfo(TelephoneGroups *tg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelephoneGroupInfo),
    isNew(true),
    telephonegroups(tg)
{
    ui->setupUi(this);
    this->telephonegroup = new TelephoneGroup();
    this->enableEdit();
}

TelephoneGroupInfo::TelephoneGroupInfo(TelephoneGroup *t, TelephoneGroups *tg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelephoneGroupInfo),
    isNew(false),
    telephonegroups(tg),
    telephonegroup(t)
{
    ui->setupUi(this);
    this->displayInfo();
    this->disableEdit();
}

TelephoneGroupInfo::~TelephoneGroupInfo()
{
    delete ui;
}

void TelephoneGroupInfo::closeEvent(QCloseEvent *e)
{
    if(!ui->groupNameLineEdit->isReadOnly())
    {
        if(this->isNew)
        {
            if(QMessageBox::information(this, "确认关闭", "是否不保存而退出编辑？",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            {
                delete this->telephonegroup;
                e->accept();
            }
            else
                e->ignore();
        }
        else
        {
            if(QMessageBox::information(this, "确认关闭", "是否不保存而退出编辑？",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
                e->accept();
            else
                e->ignore();
        }
    }
    else
        e->accept();
}

void TelephoneGroupInfo::displayInfo()
{
    ui->groupNameLineEdit->setText(QString::fromStdString(this->telephonegroup->name));
}

void TelephoneGroupInfo::saveInfo()
{
    this->telephonegroup->name = ui->groupNameLineEdit->text().toStdString();
}

void TelephoneGroupInfo::disableEdit()
{
    ui->groupNameLineEdit->setReadOnly(true);
    ui->okPushButton->hide();
}

void TelephoneGroupInfo::enableEdit()
{
    ui->groupNameLineEdit->setReadOnly(false);
    ui->okPushButton->show();
    ui->editPushButton->hide();
}

void TelephoneGroupInfo::on_editPushButton_clicked()
{
    this->enableEdit();
}

void TelephoneGroupInfo::on_okPushButton_clicked()
{
    this->disableEdit();
    this->saveInfo();
    if(this->isNew)
        emit(this->addNewTelephoneGroup(this->telephonegroup));
    this->close();
}

void TelephoneGroupInfo::on_cancelPushButton_clicked()
{
    this->disableEdit();
    if(this->isNew)
        delete this->telephonegroup;
    this->close();
}
