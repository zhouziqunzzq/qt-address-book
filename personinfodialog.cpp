#include "personinfodialog.h"
#include "ui_personinfodialog.h"
#include <QString>

using namespace std;

PersonInfoDialog::PersonInfoDialog(TelephoneGroups *tg, EmailGroups *eg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonInfoDialog),
    telephonegroups(tg),
    emailgroups(eg),
    isHidden(true)
{
    ui->setupUi(this);
    //hide more
    ui->moreWidget->hide();
    this->setFixedHeight(400);
    setWindowFlags(windowFlags()&~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint);
    //New person
    this->p = new Person();
    //setup models
    this->telModel = new QStandardItemModel(this->p->telephone.size(), 3);
    this->emailModel = new QStandardItemModel(this->p->email.size(), 3);
}

PersonInfoDialog::PersonInfoDialog(Person *person, TelephoneGroups *tg, EmailGroups *eg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonInfoDialog),
    p(person),
    telephonegroups(tg),
    emailgroups(eg),
    isHidden(true)
{
    ui->setupUi(this);
    //hide more
    ui->moreWidget->hide();
    this->setFixedHeight(400);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    //setup models
    this->telModel = new QStandardItemModel(this->p->telephone.size(), 3);
    this->emailModel = new QStandardItemModel(this->p->email.size(), 3);

    //Display person's info
    this->displayPersonInfo();

}

PersonInfoDialog::~PersonInfoDialog()
{
    delete ui;
}

void PersonInfoDialog::displayPersonInfo()
{
    //name
    ui->nameLineEdit->setText(QString::fromStdString(p->name));
    //tel
    this->setupTelModel();
    this->setupTelTableView();
    this->updateTelTableView();
    //email
    this->setupEmailModel();
    this->setupEmailTableView();
    this->updateEmailTableView();
    //birthday
    QDate birthday;
    birthday.setDate(p->birthday.year, p->birthday.month, p->birthday.day);
    ui->birthdayDateEdit->setDate(birthday);
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
    //ui->
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
        telModel->setItem(i, GroupColumn, new QStandardItem(QString::fromStdString(this->telephonegroups->findByID(it->groupID)->data.name)));
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
        emailModel->setItem(i, GroupColumn, new QStandardItem(QString::fromStdString(this->emailgroups->findByID(it->groupID)->data.name)));
        emailModel->item(i, GroupColumn)->setTextAlignment(Qt::AlignCenter);
    }
    //resize
    ui->emailTableView->resizeColumnsToContents();
}
