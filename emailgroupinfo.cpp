#include "emailgroupinfo.h"
#include "ui_emailgroupinfo.h"

EmailGroupInfo::EmailGroupInfo(EmailGroups *eg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailGroupInfo),
    emailgroups(eg),
    isNew(true)
{
    ui->setupUi(this);
    this->emailgroup = new EmailGroup();
    this->enableEdit();
}

EmailGroupInfo::EmailGroupInfo(EmailGroup *e, EmailGroups *eg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailGroupInfo),
    emailgroups(eg),
    emailgroup(e),
    isNew(false)
{
    ui->setupUi(this);
    this->displayInfo();
    this->disableEdit();
}

EmailGroupInfo::~EmailGroupInfo()
{
    delete ui;
}

void EmailGroupInfo::closeEvent(QCloseEvent *e)
{
    if(!ui->groupNameLineEdit->isReadOnly())
    {
        if(this->isNew)
        {
            if(QMessageBox::information(this, "确认关闭", "是否不保存而退出编辑？",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            {
                delete this->emailgroup;
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

void EmailGroupInfo::displayInfo()
{
    ui->groupNameLineEdit->setText(QString::fromStdString(this->emailgroup->name));
}

void EmailGroupInfo::saveInfo()
{
    this->emailgroup->name = ui->groupNameLineEdit->text().toStdString();
}

void EmailGroupInfo::disableEdit()
{
    ui->groupNameLineEdit->setReadOnly(true);
    ui->okPushButton->hide();
}

void EmailGroupInfo::enableEdit()
{
    ui->groupNameLineEdit->setReadOnly(false);
    ui->okPushButton->show();
    ui->editPushButton->hide();
}

void EmailGroupInfo::on_editPushButton_clicked()
{
    this->enableEdit();
}

void EmailGroupInfo::on_okPushButton_clicked()
{
    this->disableEdit();
    this->saveInfo();
    if(this->isNew)
        emit(this->addNewEmailGroup(this->emailgroup));
    this->close();
}

void EmailGroupInfo::on_cancelPushButton_clicked()
{
    this->disableEdit();
    if(this->isNew)
        delete this->emailgroup;
    this->close();
}
