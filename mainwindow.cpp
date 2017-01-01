#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "address.h"
#include "person.h"
#include "date.h"
#include "telephone.h"
#include <iostream>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setFixedSize(sizeHint());

    /*Settings IO Test*/
    this->ioh.readSettings(this->settings);
    /*cout << "Settings:" << endl;
    cout << this->settings.username << " " << this->settings.password << endl;*/

    if(this->settings.password != "")
    {
        //Hide MainWindow if needed
        this->setWindowOpacity(0);
        //Validate password if needed
        PasswordValidateDialog *pd = new PasswordValidateDialog(this);
        connect(pd, SIGNAL(onValidatePassword(std::string, PasswordValidateDialog*)),
                this, SLOT(validatePassword(std::string, PasswordValidateDialog*)));
        connect(pd, SIGNAL(rejected()), this, SLOT(close()));
        pd->exec();
    }

    //connections
    connect(ui->exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    /*Persons IO Test*/
    Person p1, p2, p3, p4;
    p1.id = this->persons.count();
    p1.groupID = 0;
    p1.name = "Cooler";
    p1.address = new Address("China", "Liaoning", "Shenyang", "NEU", "121212");
    p1.birthday = new Date(1997, 8, 19);
    Telephone t1("123456", 0);
    p1.telephone.push_back(t1);
    this->persons.insertAsLast(p1);

    p2.id = this->persons.count();
    p2.groupID = 1;
    p2.name = "御坂美琴";
    p2.address = new Address("China", "Liaoning", "Shenyang", "NEU", "121212");
    p2.birthday = new Date(1997, 1, 1);
    Telephone t2("654321", 0);
    p2.telephone.push_back(t2);
    this->persons.insertAsLast(p2);

    p3.id = this->persons.count();
    p3.name = "宫水三叶";
    p3.address = new Address("China", "Liaoning", "Shenyang", "Northeastern University", "2223333");
    p3.birthday = new Date(1997, 2, 2);
    Telephone t3("2323232323", 1);
    p3.telephone.push_back(t3);
    Email e1;
    e1.groupID = 0;
    e1.email = "mituha@miyami.com";
    p3.email.push_back(e1);
    e1.groupID = 1;
    e1.email = "mituha123@miyami.com";
    p3.email.push_back(e1);
    this->persons.insertAsLast(p3);

    p4.id = this->persons.count();
    p4.name = "Wang";
    p4.groupID = 1;
    p4.address = new Address("China", "Liaoning", "Shenyang", "Northeastern University", "2223333");
    p4.birthday = new Date(1997, 2, 2);
    Telephone t4("2323232323", 0);
    p4.telephone.push_back(t4);
    this->persons.insertAsLast(p4);

    this->ioh.savePersons(this->persons);
    this->persons._elem.clear();
    this->ioh.readPersons(this->persons);

    /*PersonGroups IO Test*/
    PersonGroup pg;
    pg.id = 0;
    pg.name = "亲人";
    this->persongroups.insertAsLast(pg);
    pg.id = 1;
    pg.name = "同学";
    this->persongroups.insertAsLast(pg);
    this->ioh.savePersonGroups(this->persongroups);
    this->ioh.readPersonGroups(this->persongroups);
    /*cout << "PersonGroups:" << endl;
    ListNodePosi(PersonGroup) pgit = this->persongroups._elem.first();
    for(int i = 0; i < this->persongroups.count(); ++i, pgit = pgit->succ)
    {
        cout << pgit->data.id << " " << pgit->data.name << endl;
    }*/

    /*TelephoneGroups IO Test*/
    TelephoneGroup tg;
    tg.id = 0;
    tg.name = "移动电话";
    this->telephonegroups.insertAsLast(tg);
    tg.id = 1;
    tg.name = "固定电话";
    this->telephonegroups.insertAsLast(tg);
    this->ioh.saveTelephoneGroups(this->telephonegroups);
    this->ioh.readTelephoneGroups(this->telephonegroups);
    /*cout << "TelephoneGroups:" << endl;
    ListNodePosi(TelephoneGroup) tgit = this->telephonegroups._elem.first();
    for(int i = 0; i < this->telephonegroups.count(); ++i, tgit = tgit->succ)
    {
        cout << tgit->data.id << " " << tgit->data.name << endl;
    }*/

    /*EmailGroups IO Test*/
    EmailGroup eg;
    eg.id = 0;
    eg.name = "工作";
    this->emailgroups.insertAsLast(eg);
    eg.id = 1;
    eg.name = "日常";
    this->emailgroups.insertAsLast(eg);
    this->ioh.saveEmailGroups(this->emailgroups);
    this->ioh.readEmailGroups(this->emailgroups);
    /*cout << "EmailGroups:" << endl;
    ListNodePosi(EmailGroup) egit = this->emailgroups._elem.first();
    for(int i = 0; i < this->emailgroups.count(); ++i, egit = egit->succ)
    {
        cout << egit->data.id << " " << egit->data.name << endl;
    }*/

    //Display using TableView
    this->model = new QStandardItemModel(persons.count(), 3);
    this->setupModel();
    this->setupTableView();
    this->updateTableView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::validatePassword(string pwd, PasswordValidateDialog* pd)
{
    if(this->settings.password == pwd)
    {
        pd->hide();
        delete pd;
        std::string welcomeMsg = "欢迎，" + (this->settings.username == "" ? "用户！" : this->settings.username + "！");
        QMessageBox::information(this, "登陆成功", QString::fromStdString(welcomeMsg),QMessageBox::Ok, QMessageBox::Ok);
        this->setWindowOpacity(1);
    }
    else
    {
        QMessageBox::critical(pd, "密码错误", "密码错误，请重试！", QMessageBox::Ok, QMessageBox::Ok);
    }
}

void MainWindow::on_personTableView_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        //QString s = this->model->item(index.row(), NameColumn)->text();
        //QMessageBox::information(this, "Test", s, QMessageBox::Ok);
        int id = this->model->item(index.row(), IDColumn)->text().toInt();
        PersonInfoDialog *dialog = new PersonInfoDialog(&(this->persons.findByID(id)->data),
                                                        &this->telephonegroups, &this->emailgroups, this);
        dialog->exec();
    }
}

void MainWindow::setupModel()
{
    model->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("编号"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("姓名"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("分组"));
}

void MainWindow::setupTableView()
{
    ui->personTableView->setModel(model);
    ui->personTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->personTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->personTableView->setColumnHidden(0, true);
    ui->personTableView->setSelectionBehavior(QTableView::SelectRows);
}

void MainWindow::updateTableView()
{
    //clear
    model->removeRows(0, model->rowCount());
    //sort
    this->persons.sort();
    this->persons.updateID();
    //display
    ListNodePosi(Person) pit = this->persons._elem.first();
    for(int i = 0; i < persons.count(); ++i, pit = pit->succ)
    {
        model->setItem(i, IDColumn, new QStandardItem(QString::number(pit->data.id)));
        model->item(i, IDColumn)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i, NameColumn, new QStandardItem(QString::fromStdString(pit->data.name)));
        model->item(i, NameColumn)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i, GroupColumn, new QStandardItem(QString::fromStdString(this->persongroups.findByID(pit->data.groupID)->data.name)));
        model->item(i, GroupColumn)->setTextAlignment(Qt::AlignCenter);
    }
    //resize
    //ui->personTableView->resizeColumnsToContents();
}

void MainWindow::on_pushButton_5_clicked()  //delete
{
    //Get selected row
    int selectedRow = ui->personTableView->currentIndex().row();
    //Get person's id
    int id = this->model->item(selectedRow, IDColumn)->text().toInt();
    //Confirm
    QString name = this->model->item(selectedRow, NameColumn)->text();
    if(QMessageBox::information(this, "确认删除", "确认要删除联系人" + name + "吗？",
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        //Remove and update
        this->persons.remove(this->persons.findByID(id));
        this->updateTableView();
    }
}

void MainWindow::on_personinfodialog_close(Person *newPerson)
{

}
