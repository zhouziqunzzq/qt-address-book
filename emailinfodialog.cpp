#include "emailinfodialog.h"
#include "ui_emailinfodialog.h"

EmailInfoDialog::EmailInfoDialog(EmailGroups *eg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailInfoDialog),
    emailgroups(eg),
    isNew(true)
{
    ui->setupUi(this);
    this->email = new Email();
    this->displayInfo();
    this->enableEdit();
}

EmailInfoDialog::EmailInfoDialog(Email *e, EmailGroups *eg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailInfoDialog),
    email(e),
    emailgroups(eg),
    isNew(false)

{
    ui->setupUi(this);
    this->displayInfo();
    this->enableEdit();
}

EmailInfoDialog::~EmailInfoDialog()
{
    delete ui;
}

void EmailInfoDialog::closeEvent(QCloseEvent *e)
{
    if(!ui->emailLineEdit->isReadOnly())
    {
        if(this->isNew)
        {
            if(QMessageBox::information(this, "确认关闭", "是否不保存而退出编辑？",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            {
                delete this->email;
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

void EmailInfoDialog::displayInfo()
{
    ui->emailLineEdit->setText(QString::fromStdString(this->email->email));
    if(this->email->groupID != -1)
        ui->groupLineEdit->setText(QString::fromStdString(this->emailgroups->findByID(this->email->groupID)->data.name));
    else
        ui->groupLineEdit->setText(QString::fromUtf8("未分组"));
}

void EmailInfoDialog::saveInfo()
{
    this->email->email = ui->emailLineEdit->text().toStdString();
}

void EmailInfoDialog::disableEdit()
{
    ui->emailLineEdit->setReadOnly(true);
    ui->groupSelectPushButton->setDisabled(true);
    ui->okPushButton->hide();
}

void EmailInfoDialog::enableEdit()
{
    ui->emailLineEdit->setReadOnly(false);
    ui->groupSelectPushButton->setDisabled(false);
    ui->okPushButton->show();
    ui->editPushButton->hide();
}

void EmailInfoDialog::on_editPushButton_clicked()
{
    this->enableEdit();
}

void EmailInfoDialog::on_okPushButton_clicked()
{
    this->disableEdit();
    this->saveInfo();
    if(this->isNew)
        emit(this->addNewEmail(this->email));
    this->close();
}

void EmailInfoDialog::on_cancelPushButton_clicked()
{
    this->disableEdit();
    if(this->isNew)
        delete this->email;
    this->close();
}

void EmailInfoDialog::on_groupSelectPushButton_clicked()
{
    EmailGroupsDialog *dialog = new EmailGroupsDialog(this->emailgroups, true, this);
    connect(dialog, SIGNAL(selectEmailGroup(EmailGroup*)), this, SLOT(onSelectEmailGroup(EmailGroup*)));
    dialog->exec();
}

void EmailInfoDialog::onSelectEmailGroup(EmailGroup *eg)
{
    this->email->groupID = eg->id;
    ui->groupLineEdit->setText(QString::fromStdString(eg->name));
}
