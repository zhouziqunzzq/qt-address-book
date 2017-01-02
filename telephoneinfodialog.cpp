#include "telephoneinfodialog.h"
#include "ui_telephoneinfodialog.h"

TelephoneInfoDialog::TelephoneInfoDialog(TelephoneGroups *tg, QWidget *parent) :
    QDialog(parent),
    telephonegroups(tg),
    ui(new Ui::TelephoneInfoDialog),
    isNew(true)
{
    ui->setupUi(this);
    this->telephone = new Telephone();
    this->displayInfo();
    this->enableEdit();
}

TelephoneInfoDialog::TelephoneInfoDialog(Telephone *t, TelephoneGroups *tg, QWidget *parent) :
    QDialog(parent),
    telephone(t),
    telephonegroups(tg),
    ui(new Ui::TelephoneInfoDialog),
    isNew(false)
{
    ui->setupUi(this);
    this->displayInfo();
    this->enableEdit();
}

TelephoneInfoDialog::~TelephoneInfoDialog()
{
    delete ui;
}

void TelephoneInfoDialog::closeEvent(QCloseEvent *e)
{
    if(!ui->telLineEdit->isReadOnly())
    {
        if(this->isNew)
        {
            if(QMessageBox::information(this, "确认关闭", "是否不保存而退出编辑？",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            {
                delete this->telephone;
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

void TelephoneInfoDialog::displayInfo()
{
    ui->telLineEdit->setText(QString::fromStdString(this->telephone->tel));
    if(this->telephone->groupID != -1)
        ui->groupLineEdit->setText(QString::fromStdString(this->telephonegroups->findByID(this->telephone->groupID)->data.name));
    else
        ui->groupLineEdit->setText(QString::fromUtf8("未分组"));
}

void TelephoneInfoDialog::saveInfo()
{
    this->telephone->tel = ui->telLineEdit->text().toStdString();
}

void TelephoneInfoDialog::disableEdit()
{
    ui->telLineEdit->setReadOnly(true);
    ui->groupSelectPushButton->setDisabled(true);
    ui->okPushButton->hide();
}

void TelephoneInfoDialog::enableEdit()
{
    ui->telLineEdit->setReadOnly(false);
    ui->groupSelectPushButton->setDisabled(false);
    ui->okPushButton->show();
    ui->editPushButton->hide();
}

void TelephoneInfoDialog::on_editPushButton_clicked()
{
    this->enableEdit();
}

void TelephoneInfoDialog::on_okPushButton_clicked()
{
    this->disableEdit();
    this->saveInfo();
    if(this->isNew)
        emit(this->addNewTelephone(this->telephone));
    this->close();
}

void TelephoneInfoDialog::on_cancelPushButton_clicked()
{
    this->disableEdit();
    if(this->isNew)
        delete this->telephone;
    this->close();
}

void TelephoneInfoDialog::on_groupSelectPushButton_clicked()
{
    TelephoneGroupsDialog *dialog = new TelephoneGroupsDialog(this->telephonegroups, true, this);
    connect(dialog, SIGNAL(selectTelephoneGroup(TelephoneGroup*)), this, SLOT(onSelectTelephoneGroup(TelephoneGroup*)));
    dialog->exec();
}

void TelephoneInfoDialog::onSelectTelephoneGroup(TelephoneGroup *tg)
{
    this->telephone->groupID = tg->id;
    ui->groupLineEdit->setText(QString::fromStdString(tg->name));
}
