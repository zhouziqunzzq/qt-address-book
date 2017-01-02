#include "persongroupinfo.h"
#include "ui_persongroupinfo.h"

PersonGroupInfo::PersonGroupInfo(PersonGroups *pg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonGroupInfo),
    persongroups(pg),
    isNew(true)
{
    ui->setupUi(this);
    this->persongroup = new PersonGroup();
    this->enableEdit();
}

PersonGroupInfo::PersonGroupInfo(PersonGroup *p, PersonGroups *pg, QWidget *parent):
    QDialog(parent),
    ui(new Ui::PersonGroupInfo),
    persongroups(pg),
    persongroup(p),
    isNew(false)
{
    ui->setupUi(this);
    this->displayInfo();
    this->disableEdit();
}

PersonGroupInfo::~PersonGroupInfo()
{
    delete ui;
}

void PersonGroupInfo::closeEvent(QCloseEvent *e)
{
    if(!ui->groupNameLineEdit->isReadOnly())
    {
        if(this->isNew)
        {
            if(QMessageBox::information(this, "确认关闭", "是否不保存而退出编辑？",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            {
                delete this->persongroup;
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

void PersonGroupInfo::displayInfo()
{
    ui->groupNameLineEdit->setText(QString::fromStdString(this->persongroup->name));
}

void PersonGroupInfo::saveInfo()
{
    this->persongroup->name = ui->groupNameLineEdit->text().toStdString();
}

void PersonGroupInfo::disableEdit()
{
    ui->groupNameLineEdit->setReadOnly(true);
    ui->okPushButton->hide();
}

void PersonGroupInfo::enableEdit()
{
    ui->groupNameLineEdit->setReadOnly(false);
    ui->okPushButton->show();
    ui->editPushButton->hide();
}

void PersonGroupInfo::on_editPushButton_clicked()
{
    this->enableEdit();
}

void PersonGroupInfo::on_okPushButton_clicked()
{
    this->disableEdit();
    this->saveInfo();
    if(this->isNew)
        emit(this->addNewPersonGroup(this->persongroup));
    this->close();
}

void PersonGroupInfo::on_cancelPushButton_clicked()
{
    this->disableEdit();
    if(this->isNew)
        delete this->persongroup;
    this->close();
}
