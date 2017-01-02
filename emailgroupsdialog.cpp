#include "emailgroupsdialog.h"
#include "ui_emailgroupsdialog.h"

EmailGroupsDialog::EmailGroupsDialog(EmailGroups *eg, bool isSelection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailGroupsDialog),
    emailgroups(eg)
{
    ui->setupUi(this);
    if(!isSelection)
    {
        ui->okPushButton->hide();
        ui->cancelPushButton->hide();
    }
    else
    {
        ui->delPushButton->hide();
    }
    this->model = new QStandardItemModel(this->emailgroups->count(), 2);
    this->setupModel();
    this->setupTableView();
    this->updateTableView();
}

EmailGroupsDialog::~EmailGroupsDialog()
{
    delete ui;
}

void EmailGroupsDialog::setupModel()
{
    model->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("编号"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("组名"));
}

void EmailGroupsDialog::setupTableView()
{
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
}

void EmailGroupsDialog::updateTableView()
{
    //clear
    model->removeRows(0, model->rowCount());
    //display
    ListNodePosi(EmailGroup) pit = this->emailgroups->_elem.first();
    for(int i = 0; i < emailgroups->count(); ++i, pit = pit->succ)
    {
        model->setItem(i, IDColumn, new QStandardItem(QString::number(pit->data.id)));
        model->item(i, IDColumn)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i, NameColumn, new QStandardItem(QString::fromStdString(pit->data.name)));
        model->item(i, NameColumn)->setTextAlignment(Qt::AlignCenter);
    }
    //resize
    //ui->personTableView->resizeColumnsToContents();
}

void EmailGroupsDialog::onAddNewEmailGroup(EmailGroup *eg)
{
    eg->id = ++this->emailgroups->maxID;
    this->emailgroups->insertAsLast(*eg);
    delete eg;
    this->updateTableView();
}

void EmailGroupsDialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        int id = this->model->item(index.row(), IDColumn)->text().toInt();
        EmailGroupInfo *dialog = new EmailGroupInfo(&(this->emailgroups->findByID(id)->data), this->emailgroups, this);
        dialog->exec();
        this->updateTableView();
        delete dialog;
    }
}

void EmailGroupsDialog::on_newPushButton_clicked()
{
    EmailGroupInfo *dialog = new EmailGroupInfo(this->emailgroups, this);
    connect(dialog, SIGNAL(addNewEmailGroup(EmailGroup*)), this, SLOT(onAddNewEmailGroup(EmailGroup*)));
    dialog->exec();
    delete dialog;
}

void EmailGroupsDialog::on_delPushButton_clicked()
{
    if(ui->tableView->currentIndex().row() == -1)
        return;
    //Get selected row
    int selectedRow = ui->tableView->currentIndex().row();
    //Get id
    int id = this->model->item(selectedRow, IDColumn)->text().toInt();
    //Get pointer
    ListNodePosi(EmailGroup) g = this->emailgroups->findByID(id);
    //Get name
    QString name = this->model->item(selectedRow, NameColumn)->text();
    QString text = "确认要删除";
    if(g->data.count != 0)
        text += "非空";
    text = text + "分组" + name + "吗？";
    //Confirm
    if(QMessageBox::information(this, "确认删除", text,
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        //Remove and update
        if(g->data.count != 0)
            emit(this->cleanEmailGroup(&(g->data)));
        this->emailgroups->remove(g);
        this->updateTableView();
    }
}

void EmailGroupsDialog::on_okPushButton_clicked()
{
    //Get selected row
    int selectedRow = ui->tableView->currentIndex().row();
    if(selectedRow < 0)
    {
        QMessageBox::information(this, "提示", "请选择一个分组！", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    //Get id
    int id = this->model->item(selectedRow, IDColumn)->text().toInt();
    emit(this->selectEmailGroup(&(this->emailgroups->findByID(id)->data)));
    this->close();
}

void EmailGroupsDialog::on_cancelPushButton_clicked()
{
    this->close();
}
