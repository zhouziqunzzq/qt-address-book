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
    ui(new Ui::MainWindow),
    needSave(false)
{
    ui->setupUi(this);

    /*Settings IO Test*/
    this->ioh.readSettings(this->settings);

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
        delete pd;
    }

    //connections
    connect(ui->exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    //Generate test data
    this->generateTestData();

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

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(this->needSave)
    {
        auto rst = QMessageBox::information(this, "提示", "是否保存修改到文件？",
                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        switch(rst)
        {
        case QMessageBox::Yes:
            this->on_saveAction_triggered();
            e->accept();
            break;
        case QMessageBox::No:
            e->accept();
            break;
        case QMessageBox::Cancel:
            e->ignore();
            break;
        }
    }
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
        this->needSave = true;
        int id = this->model->item(index.row(), IDColumn)->text().toInt();
        PersonInfoDialog *dialog = new PersonInfoDialog(&(this->persons.findByID(id)->data), &this->persongroups,
                                                        &this->telephonegroups, &this->emailgroups, this);
        connect(dialog, SIGNAL(needUpdatePersonsView()), this, SLOT(updateTableView()));
        dialog->exec();
        delete dialog;
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

void MainWindow::generateTestData()
{
    /*PersonGroups IO Test*/
    PersonGroup pg;
    pg.id = 0;
    pg.name = "亲人";
    this->persongroups.insertAsLast(pg);
    pg.id = 1;
    pg.name = "同学";
    this->persongroups.insertAsLast(pg);
    this->ioh.savePersonGroups(this->persongroups);
    this->persongroups._elem.clear();
    this->ioh.readPersonGroups(this->persongroups);

    /*TelephoneGroups IO Test*/
    TelephoneGroup tg;
    tg.id = 0;
    tg.name = "移动电话";
    this->telephonegroups.insertAsLast(tg);
    tg.id = 1;
    tg.name = "固定电话";
    this->telephonegroups.insertAsLast(tg);
    this->ioh.saveTelephoneGroups(this->telephonegroups);
    this->telephonegroups._elem.clear();
    this->ioh.readTelephoneGroups(this->telephonegroups);

    /*EmailGroups IO Test*/
    EmailGroup eg;
    eg.id = 0;
    eg.name = "工作";
    this->emailgroups.insertAsLast(eg);
    eg.id = 1;
    eg.name = "日常";
    this->emailgroups.insertAsLast(eg);
    this->ioh.saveEmailGroups(this->emailgroups);
    this->emailgroups._elem.clear();
    this->ioh.readEmailGroups(this->emailgroups);

    /*Persons IO Test*/
    Person p1, p2, p3, p4;
    p1.id = this->persons.count();
    p1.groupID = 0;
    ++this->persongroups.findByID(p1.groupID)->data.count;
    p1.name = "Cooler";
    p1.address = new Address("China", "Liaoning", "Shenyang", "NEU");
    p1.birthday = new Date(1997, 8, 19);
    Telephone t1("123456", 0);
    ++this->telephonegroups.findByID(t1.groupID)->data.count;
    p1.telephone.push_back(t1);
    this->persons.insertAsLast(p1);

    p2.id = this->persons.count();
    p2.groupID = 1;
    ++this->persongroups.findByID(p2.groupID)->data.count;
    p2.name = "御坂美琴";
    p2.address = new Address("China", "Liaoning", "Shenyang", "NEU");
    p2.birthday = new Date(1997, 1, 1);
    Telephone t2("654321", 0);
    ++this->telephonegroups.findByID(t2.groupID)->data.count;
    p2.telephone.push_back(t2);
    this->persons.insertAsLast(p2);

    p3.id = this->persons.count();
    p3.groupID = -1;
    p3.name = "宫水三叶";
    p3.address = new Address("China", "Liaoning", "Shenyang", "Northeastern University");
    p3.birthday = new Date(1997, 2, 2);
    Telephone t3("2323232323", 1);
    ++this->telephonegroups.findByID(t3.groupID)->data.count;
    p3.telephone.push_back(t3);
    Email e1;
    e1.groupID = -1;
    e1.email = "mituha@miyami.com";
    p3.email.push_back(e1);
    e1.groupID = 1;
    e1.email = "mituha123@miyami.com";
    ++this->emailgroups.findByID(e1.groupID)->data.count;
    p3.email.push_back(e1);
    this->persons.insertAsLast(p3);

    p4.id = this->persons.count();
    p4.name = "Wang";
    p4.groupID = 1;
    ++this->persongroups.findByID(p4.groupID)->data.count;
    p4.address = new Address("China", "Liaoning", "Shenyang", "Northeastern University");
    p4.birthday = new Date(1997, 2, 2);
    Telephone t4("2323232323", -1);
    p4.telephone.push_back(t4);
    this->persons.insertAsLast(p4);

    this->ioh.savePersons(this->persons);
    this->persons._elem.clear();
    this->ioh.readPersons(this->persons);
}

void MainWindow::updateTableView()
{
    //clear
    model->removeRows(0, model->rowCount());
    //sort
    this->persons.sort();
    //display
    ListNodePosi(Person) pit = this->persons._elem.first();
    for(int i = 0; i < persons.count(); ++i, pit = pit->succ)
    {
        model->setItem(i, IDColumn, new QStandardItem(QString::number(pit->data.id)));
        model->item(i, IDColumn)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i, NameColumn, new QStandardItem(QString::fromStdString(pit->data.name)));
        model->item(i, NameColumn)->setTextAlignment(Qt::AlignCenter);
        if(pit->data.groupID != -1)
            model->setItem(i, GroupColumn, new QStandardItem(QString::fromStdString(this->persongroups.findByID(pit->data.groupID)->data.name)));
        else
            model->setItem(i, GroupColumn, new QStandardItem(QString::fromUtf8("未分组")));
        model->item(i, GroupColumn)->setTextAlignment(Qt::AlignCenter);
    }
    //resize
    //ui->personTableView->resizeColumnsToContents();
    //update count
    this->updateCount();
}

void MainWindow::updateCount()
{
    //Clean persongroups count
    ListNodePosi(PersonGroup) pgit = this->persongroups._elem.first();
    for(int i = 0; i < this->persongroups.count(); ++i, pgit = pgit->succ)
        pgit->data.count = 0;
    //Clean telephonegroups count
    ListNodePosi(TelephoneGroup) tgit = this->telephonegroups._elem.first();
    for(int i = 0; i < this->telephonegroups.count(); ++i, tgit = tgit->succ)
        tgit->data.count = 0;
    //Clean emailgroups count
    ListNodePosi(EmailGroup) egit = this->emailgroups._elem.first();
    for(int i = 0; i < this->emailgroups.count(); ++i, egit = egit->succ)
        egit->data.count = 0;
    //Update count
    ListNodePosi(Person) pit = this->persons._elem.first();
    for(int i = 0; i < this->persons.count(); ++i, pit = pit->succ)
    {
        if(pit->data.groupID != -1)
            ++this->persongroups.findByID(pit->data.groupID)->data.count;
        for(vector<Telephone>::iterator it = pit->data.telephone.begin();
            it != pit->data.telephone.end(); ++it)
        {
            if(it->groupID != -1)
                ++this->telephonegroups.findByID(it->groupID)->data.count;
        }
        for(vector<Email>::iterator it = pit->data.email.begin();
            it != pit->data.email.end(); ++it)
        {
            if(it->groupID != -1)
                ++this->emailgroups.findByID(it->groupID)->data.count;
        }
    }
}

void MainWindow::on_pushButton_5_clicked()  //delete
{
    if(ui->personTableView->currentIndex().row() == -1)
        return;
    //Get selected row
    int selectedRow = ui->personTableView->currentIndex().row();
    //Get person's id
    int id = this->model->item(selectedRow, IDColumn)->text().toInt();
    //Get pointer
    ListNodePosi(Person) p =this->persons.findByID(id);
    //Confirm
    QString name = this->model->item(selectedRow, NameColumn)->text();
    if(QMessageBox::information(this, "确认删除", "确认要删除联系人" + name + "吗？",
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        //Remove and update
        this->needSave = true;
        this->persons.remove(p);
        this->updateTableView();
    }
}

void MainWindow::onAddnewperson(Person *newPerson)
{
    this->needSave = true;
    newPerson->id = ++this->persons.maxID;
    this->persons.insertAsLast(*newPerson);
    delete newPerson;
    this->updateTableView();
}

void MainWindow::on_newPushButton_clicked()
{
    PersonInfoDialog *dialog = new PersonInfoDialog(&this->persongroups, &this->telephonegroups,
                                                    &this->emailgroups, this);
    connect(dialog, SIGNAL(addNewPerson(Person*)), this, SLOT(onAddnewperson(Person*)));
    dialog->exec();
    delete dialog;
}

void MainWindow::on_saveAction_triggered()
{
    bool flag = true;
    flag &= this->ioh.savePersons(this->persons);
    flag &= this->ioh.savePersonGroups(this->persongroups);
    flag &= this->ioh.saveTelephoneGroups(this->telephonegroups);
    flag &= this->ioh.saveEmailGroups(this->emailgroups);
    flag &= this->ioh.saveSettings(this->settings);
    if(flag)
    {
        QMessageBox::information(this, "提示", "保存成功！", QMessageBox::Ok, QMessageBox::Ok);
        this->needSave = false;
    }
    else
        QMessageBox::critical(this, "错误", "保存失败，请重试！", QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_personGroupsManageAction_triggered()
{
    this->needSave = true;
    PersonGroupsDialog *dialog = new PersonGroupsDialog(&(this->persongroups), false, this);
    connect(dialog, SIGNAL(cleanPersonGroup(PersonGroup*)), this, SLOT(onCleanPersonGroup(PersonGroup*)));
    connect(dialog, SIGNAL(clearPersonGroup(PersonGroup*)), this, SLOT(onClearPersonGroup(PersonGroup*)));
    dialog->exec();
    this->updateTableView();
    delete dialog;
}

void MainWindow::on_telGroupsManageAction_triggered()
{
    this->needSave = true;
    TelephoneGroupsDialog *dialog = new TelephoneGroupsDialog(&(this->telephonegroups), false, this);
    connect(dialog, SIGNAL(cleanTelephoneGroup(TelephoneGroup*)), this, SLOT(onCleanTelephoneGroup(TelephoneGroup*)));
    dialog->exec();
    this->updateTableView();
    delete dialog;
}

void MainWindow::on_emailGroupsManageAction_triggered()
{
    this->needSave = true;
    EmailGroupsDialog *dialog = new EmailGroupsDialog(&(this->emailgroups), false, this);
    connect(dialog, SIGNAL(cleanEmailGroup(EmailGroup*)), this, SLOT(onCleanEmailGroup(EmailGroup*)));
    dialog->exec();
    this->updateTableView();
    delete dialog;
}

void MainWindow::on_settingsManageAction_triggered()
{
    this->needSave = true;

}

void MainWindow::onCleanEmailGroup(EmailGroup *eg)
{
    ListNodePosi(Person) pit = this->persons._elem.first();
    for(int i = 0; i < this->persons.count(); ++i, pit = pit->succ)
    {
        for(vector<Email>::iterator it = pit->data.email.begin();
            it != pit->data.email.end(); ++it)
        {
            if(it->groupID == eg->id)
                it->groupID = -1;
        }
    }
}

void MainWindow::onCleanTelephoneGroup(TelephoneGroup *tg)
{
    ListNodePosi(Person) pit = this->persons._elem.first();
    for(int i = 0; i < this->persons.count(); ++i, pit = pit->succ)
    {
        for(vector<Telephone>::iterator it = pit->data.telephone.begin();
            it != pit->data.telephone.end(); ++it)
        {
            if(it->groupID == tg->id)
                it->groupID = -1;
        }
    }
}

void MainWindow::onCleanPersonGroup(PersonGroup *pg)
{
    ListNodePosi(Person) pit = this->persons._elem.first();
    for(int i = 0; i < this->persons.count(); ++i, pit = pit->succ)
    {
        if(pit->data.groupID == pg->id)
            pit->data.groupID = -1;
    }
    this->updateTableView();
}

void MainWindow::onClearPersonGroup(PersonGroup *pg)
{
    ListNodePosi(Person) pit = this->persons._elem.first();
    ListNodePosi(Person) q = NULL;
    int cnt = this->persons.count();
    for(int i = 0; i < cnt; ++i, pit = q)
    {
        q = pit->succ;
        if(pit->data.groupID == pg->id)
            this->persons.remove(pit);
    }
    this->updateTableView();
}

void MainWindow::on_pushButton_clicked()
{
    QString keyword = ui->keywordLineEdit->text();
    for(int i = 0; i < this->persons.count(); ++i)
    {
        if(!this->model->item(i, NameColumn)->text().contains(keyword))
            ui->personTableView->setRowHidden(i, true);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->keywordLineEdit->clear();
    for(int i = 0; i < this->persons.count(); ++i)
        ui->personTableView->setRowHidden(i, false);
}

void MainWindow::on_keywordLineEdit_textChanged(const QString &arg1)
{
    for(int i = 0; i < this->persons.count(); ++i)
        ui->personTableView->setRowHidden(i, false);
    if(ui->keywordLineEdit->text().count() != 0)
        this->on_pushButton_clicked();
}

void MainWindow::on_pushButton_3_clicked()
{
    PersonGroupsDialog *dialog = new PersonGroupsDialog(&(this->persongroups), true, this);
    connect(dialog, SIGNAL(selectPersonGroup(PersonGroup*)), this, SLOT(onPersonGroupSelect(PersonGroup*)));
    dialog->exec();
    delete dialog;
}

void MainWindow::onPersonGroupSelect(PersonGroup *pg)
{
    QString keyword = QString::fromStdString(pg->name);
    ui->groupFilterLabel->setText(keyword);
    for(int i = 0; i < this->persons.count(); ++i)
    {
        if(!this->model->item(i, GroupColumn)->text().contains(keyword))
            ui->personTableView->setRowHidden(i, true);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->groupFilterLabel->setText(QString::fromUtf8("全部分组"));
    for(int i = 0; i < this->persons.count(); ++i)
        ui->personTableView->setRowHidden(i, false);
}
