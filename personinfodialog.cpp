#include "personinfodialog.h"
#include "ui_personinfodialog.h"
#include <QString>

using namespace std;

PersonInfoDialog::PersonInfoDialog(PersonGroups *pg, TelephoneGroups *tg, EmailGroups *eg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonInfoDialog),
    persongroups(pg),
    telephonegroups(tg),
    emailgroups(eg),
    isHidden(true),
    isNew(true)
{
    ui->setupUi(this);
    //hide more
    ui->moreWidget->hide();
    this->setFixedHeight(400);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    //New person
    this->p = new Person();
    //setup models
    this->telModel = new QStandardItemModel(this->p->telephone.size(), 3);
    this->emailModel = new QStandardItemModel(this->p->email.size(), 3);
    //enable edit
    this->enableEdit();
}

PersonInfoDialog::PersonInfoDialog(Person *person, PersonGroups *pg, TelephoneGroups *tg, EmailGroups *eg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonInfoDialog),
    p(person),
    persongroups(pg),
    telephonegroups(tg),
    emailgroups(eg),
    isHidden(true),
    isNew(false)
{
    ui->setupUi(this);
    //Hide more
    ui->moreWidget->hide();
    this->setFixedHeight(400);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    //New Temp Person
    this->tempPerson = new Person(*person);

    //Setup models
    this->telModel = new QStandardItemModel(this->p->telephone.size(), 3);
    this->emailModel = new QStandardItemModel(this->p->email.size(), 3);

    //Disable edit
    this->disableEdit();

    //Display person's info
    this->displayInfo();

}

PersonInfoDialog::~PersonInfoDialog()
{
    delete tempPerson;
    delete ui;
}

void PersonInfoDialog::closeEvent(QCloseEvent *e)
{
    if(!ui->nameLineEdit->isReadOnly())
    {
        if(this->isNew)
        {
            if(QMessageBox::information(this, "确认关闭", "是否不保存而退出编辑？",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            {
                delete this->p;
                e->accept();
            }
            else
                e->ignore();
        }
        else
        {
            if(QMessageBox::information(this, "确认关闭", "是否不保存而退出编辑？",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            {
                *(this->p) = *(this->tempPerson);
                e->accept();
            }
            else
                e->ignore();
        }
    }
    else
        e->accept();
}

void PersonInfoDialog::displayInfo()
{
    //name
    ui->nameLineEdit->setText(QString::fromStdString(p->name));
    //group
    if(p->groupID != -1)
        ui->groupLineEdit->setText(QString::fromStdString(this->persongroups->findByID(p->groupID)->data.name));
    else
        ui->groupLineEdit->setText("未分组");
    //tel
    this->setupTelModel();
    this->setupTelTableView();
    this->updateTelTableView();
    //email
    this->setupEmailModel();
    this->setupEmailTableView();
    this->updateEmailTableView();
    //company
    ui->companyLineEdit->setText(QString::fromStdString(this->p->company));
    //address
    ui->countryLineEdit->setText(QString::fromStdString(this->p->address.country));
    ui->provinceLineEdit->setText(QString::fromStdString(this->p->address.province));
    ui->cityLineEdit->setText(QString::fromStdString(this->p->address.city));
    ui->detailLineEdit->setText(QString::fromStdString(this->p->address.detail));
    //birthday
    QDate birthday;
    birthday.setDate(p->birthday.year, p->birthday.month, p->birthday.day);
    ui->birthdayDateEdit->setDate(birthday);
    //memo
    ui->memoTextEdit->setText(QString::fromStdString(this->p->memo));
}

void PersonInfoDialog::saveInfo()
{
    //name
    this->p->name = ui->nameLineEdit->text().toStdString();
    //company
    this->p->company = ui->companyLineEdit->text().toStdString();
    //address
    this->p->address.country = ui->countryLineEdit->text().toStdString();
    this->p->address.province = ui->provinceLineEdit->text().toStdString();
    this->p->address.city = ui->cityLineEdit->text().toStdString();
    this->p->address.detail = ui->detailLineEdit->toPlainText().toStdString();
    //birthday
    this->p->birthday.year = ui->birthdayDateEdit->date().year();
    this->p->birthday.month = ui->birthdayDateEdit->date().month();
    this->p->birthday.day = ui->birthdayDateEdit->date().day();
    //memo
    this->p->memo = ui->memoTextEdit->toPlainText().toStdString();
}

void PersonInfoDialog::on_morePushButton_clicked()
{
    if(isHidden)
    {
        isHidden = false;
        ui->morePushButton->setText("更少");
        ui->moreWidget->show();
        this->setFixedHeight(600);
    }
    else
    {
        isHidden = true;
        ui->morePushButton->setText("更多");
        ui->moreWidget->hide();
        this->setFixedHeight(400);
    }
}

void PersonInfoDialog::disableEdit()
{
    //name
    ui->nameLineEdit->setReadOnly(true);
    //group
    ui->changeGroupPushButton->setDisabled(true);
    //tel
    disconnect(ui->telTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_telTableView_doubleClicked(QModelIndex)));
    ui->addTelPushButton->setDisabled(true);
    ui->delTelPushButton->setDisabled(true);
    //email
    disconnect(ui->emailTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_emailTableView_doubleClicked(QModelIndex)));
    ui->addEmailPushButton->setDisabled(true);
    ui->delEmailPushButton->setDisabled(true);
    //company
    ui->companyLineEdit->setReadOnly(true);
    //address
    ui->countryLineEdit->setReadOnly(true);
    ui->provinceLineEdit->setReadOnly(true);
    ui->cityLineEdit->setReadOnly(true);
    ui->detailLineEdit->setReadOnly(true);
    //birthday
    ui->birthdayDateEdit->setReadOnly(true);
    //memo
    ui->memoTextEdit->setReadOnly(true);
    //ok
    ui->OkPushButton->hide();
}

void PersonInfoDialog::enableEdit()
{
    //name
    ui->nameLineEdit->setReadOnly(false);
    //group
    ui->changeGroupPushButton->setDisabled(false);
    //tel
    connect(ui->telTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_telTableView_doubleClicked(QModelIndex)));
    ui->addTelPushButton->setDisabled(false);
    ui->delTelPushButton->setDisabled(false);
    //email
    connect(ui->emailTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_emailTableView_doubleClicked(QModelIndex)));
    ui->addEmailPushButton->setDisabled(false);
    ui->delEmailPushButton->setDisabled(false);
    //company
    ui->companyLineEdit->setReadOnly(false);
    //address
    ui->countryLineEdit->setReadOnly(false);
    ui->provinceLineEdit->setReadOnly(false);
    ui->cityLineEdit->setReadOnly(false);
    ui->detailLineEdit->setReadOnly(false);
    //birthday
    ui->birthdayDateEdit->setReadOnly(false);
    //memo
    ui->memoTextEdit->setReadOnly(false);
    //ok
    ui->OkPushButton->show();
    //edit
    ui->editPushButton->hide();
}

void PersonInfoDialog::setupTelModel()
{
    telModel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("编号"));
    telModel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("号码"));
    telModel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("分组"));
}

void PersonInfoDialog::setupTelTableView()
{
    ui->telTableView->setModel(telModel);
    ui->telTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->telTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->telTableView->setColumnHidden(0, true);
    ui->telTableView->setSelectionBehavior(QTableView::SelectRows);
}

void PersonInfoDialog::updateTelTableView()
{
    //clear
    telModel->removeRows(0, telModel->rowCount());
    //display
    vector<Telephone>::iterator it = this->p->telephone.begin();
    for(int i = 0; it != this->p->telephone.end(); ++i, ++it)
    {
        telModel->setItem(i, IDColumn, new QStandardItem(QString::number(i)));
        telModel->item(i, IDColumn)->setTextAlignment(Qt::AlignCenter);
        telModel->setItem(i, TelColumn, new QStandardItem(QString::fromStdString(it->tel)));
        telModel->item(i, TelColumn)->setTextAlignment(Qt::AlignCenter);
        if(it->groupID != -1)
            telModel->setItem(i, GroupColumn, new QStandardItem(QString::fromStdString(this->telephonegroups->findByID(it->groupID)->data.name)));
        else
            telModel->setItem(i, GroupColumn, new QStandardItem(QString::fromUtf8("未分组")));
        telModel->item(i, GroupColumn)->setTextAlignment(Qt::AlignCenter);
    }
    //resize
    ui->telTableView->resizeColumnsToContents();
}

void PersonInfoDialog::setupEmailModel()
{
    emailModel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("编号"));
    emailModel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("地址"));
    emailModel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("分组"));
}

void PersonInfoDialog::setupEmailTableView()
{
    ui->emailTableView->setModel(emailModel);
    ui->emailTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->emailTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->emailTableView->setColumnHidden(0, true);
    ui->emailTableView->setSelectionBehavior(QTableView::SelectRows);
}

void PersonInfoDialog::updateEmailTableView()
{
    //clear
    emailModel->removeRows(0, emailModel->rowCount());
    //display
    vector<Email>::iterator it = this->p->email.begin();
    for(int i = 0; it != this->p->email.end(); ++i, ++it)
    {
        emailModel->setItem(i, IDColumn, new QStandardItem(QString::number(i)));
        emailModel->item(i, IDColumn)->setTextAlignment(Qt::AlignCenter);
        emailModel->setItem(i, TelColumn, new QStandardItem(QString::fromStdString(it->email)));
        emailModel->item(i, TelColumn)->setTextAlignment(Qt::AlignCenter);
        if(it->groupID != -1)
            emailModel->setItem(i, GroupColumn, new QStandardItem(QString::fromStdString(this->emailgroups->findByID(it->groupID)->data.name)));
        else
            emailModel->setItem(i, GroupColumn, new QStandardItem(QString::fromUtf8("未分组")));
        emailModel->item(i, GroupColumn)->setTextAlignment(Qt::AlignCenter);
    }
    //resize
    ui->emailTableView->resizeColumnsToContents();
}

void PersonInfoDialog::on_telTableView_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        int id = this->telModel->item(index.row(), IDColumn)->text().toInt();
        TelephoneInfoDialog *dialog = new TelephoneInfoDialog(&this->p->telephone.at(id), this->telephonegroups, this);
        dialog->exec();
        this->updateTelTableView();
    }
}

void PersonInfoDialog::on_emailTableView_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        int id = this->emailModel->item(index.row(), IDColumn)->text().toInt();
        EmailInfoDialog *dialog = new EmailInfoDialog(&this->p->email.at(id), this->emailgroups, this);
        dialog->exec();
        this->updateEmailTableView();
    }
}

void PersonInfoDialog::on_cancelPushButton_clicked()
{
    this->disableEdit();
    if(this->isNew)
        delete this->p;
    else
    {
        //Recover
        *(this->p) = *(this->tempPerson);
    }
    this->close();
}

void PersonInfoDialog::on_editPushButton_clicked()
{
    this->enableEdit();
}

void PersonInfoDialog::on_OkPushButton_clicked()
{
    this->disableEdit();
    this->saveInfo();
    emit(this->needUpdatePersonsView());
    if(this->isNew)
        emit(this->addNewPerson(this->p));
    this->close();
}

void PersonInfoDialog::on_addTelPushButton_clicked()
{
    TelephoneInfoDialog *dialog = new TelephoneInfoDialog(this->telephonegroups, this);
    connect(dialog, SIGNAL(addNewTelephone(Telephone*)), this, SLOT(onAddNewTel(Telephone*)));
    dialog->exec();
}

void PersonInfoDialog::onAddNewTel(Telephone *t)
{
    this->p->telephone.push_back(*t);
    delete t;
    this->updateTelTableView();
}

void PersonInfoDialog::on_delTelPushButton_clicked()
{
    if(ui->telTableView->currentIndex().row() == -1)
        return;
    //Get selected row
    int selectedRow = ui->telTableView->currentIndex().row();
    //Get id
    int id = this->telModel->item(selectedRow, IDColumn)->text().toInt();
    //Confirm
    QString tel = this->telModel->item(selectedRow, TelColumn)->text();
    if(QMessageBox::information(this, "确认删除", "确认要删除电话:" + tel + "吗？",
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        //Remove and update
        if(p->telephone.at(id).groupID != -1)
            --this->telephonegroups->findByID(p->telephone.at(id).groupID)->data.count;
        this->p->telephone.erase(this->p->telephone.begin() + id);

        this->updateTelTableView();
    }
}

void PersonInfoDialog::on_addEmailPushButton_clicked()
{
    EmailInfoDialog *dialog = new EmailInfoDialog(this->emailgroups, this);
    connect(dialog, SIGNAL(addNewEmail(Email*)), this, SLOT(onAddNewEmail(Email*)));
    dialog->exec();
}

void PersonInfoDialog::onAddNewEmail(Email *e)
{
    this->p->email.push_back(*e);
    delete e;
    this->updateEmailTableView();
}

void PersonInfoDialog::on_delEmailPushButton_clicked()
{
    if(ui->emailTableView->currentIndex().row() == -1)
        return;
    //Get selected row
    int selectedRow = ui->emailTableView->currentIndex().row();
    //Get id
    int id = this->emailModel->item(selectedRow, IDColumn)->text().toInt();
    //Confirm
    QString tel = this->emailModel->item(selectedRow, TelColumn)->text();
    if(QMessageBox::information(this, "确认删除", "确认要删除Email:" + tel + "吗？",
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        //Remove and update
        if(p->email.at(id).groupID != -1)
            --this->emailgroups->findByID(p->email.at(id).groupID)->data.count;
        this->p->email.erase(this->p->email.begin() + id);
        this->updateEmailTableView();
    }
}

void PersonInfoDialog::on_changeGroupPushButton_clicked()
{
    PersonGroupsDialog *dialog = new PersonGroupsDialog(this->persongroups, true, this);
    connect(dialog, SIGNAL(selectPersonGroup(PersonGroup*)), this, SLOT(onSelectPersonGroup(PersonGroup*)));
    dialog->exec();
}

void PersonInfoDialog::onSelectPersonGroup(PersonGroup *pg)
{
    if(this->p->groupID != -1)
        --this->persongroups->findByID(p->groupID)->data.count;
    this->p->groupID = pg->id;
    if(pg->id != -1)
        ++this->persongroups->findByID(pg->id)->data.count;
    ui->groupLineEdit->setText(QString::fromStdString(pg->name));
}
