#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(Settings *s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    settings(s)
{
    ui->setupUi(this);
    this->setFixedSize(sizeHint());
    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    ui->repwdLineEdit->setEchoMode(QLineEdit::Password);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->displayInfo();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::displayInfo()
{
    ui->usernameLineEdit->setText(QString::fromStdString(settings->username));
}

void SettingsDialog::saveInfo()
{
    this->settings->username = ui->usernameLineEdit->text().toStdString();
    if(ui->pwdLineEdit->text().count() != 0 || ui->repwdLineEdit->text().count() != 0)
    {
        if(ui->pwdLineEdit->text() != ui->repwdLineEdit->text())
        {
            QMessageBox::information(this, "提示", "两次输入的密码不一致，请检查！", QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        else
            this->settings->password = ui->pwdLineEdit->text().toStdString();
    }
    else if(ui->checkBox->isChecked())
        this->settings->password = "";
    this->close();
}

void SettingsDialog::on_okPushButton_clicked()
{
    this->saveInfo();
}

void SettingsDialog::on_checkBox_clicked()
{
    ui->pwdLineEdit->clear();
    ui->repwdLineEdit->clear();
}
