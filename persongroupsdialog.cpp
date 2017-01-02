#include "persongroupsdialog.h"
#include "ui_persongroupsdialog.h"

PersonGroupsDialog::PersonGroupsDialog(PersonGroups *pg, bool isSelection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonGroupsDialog),
    persongroups(pg)
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
    this->model = new QStandardItemModel(this->persongroups->count(), 3);
    this->setupModel();
    this->setupTableView();
    this->updateTableView();
}

PersonGroupsDialog::~PersonGroupsDialog()
{
    delete ui;
}

void PersonGroupsDialog::setupModel()
{
    model->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("编号"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("组名"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("记录数"));
}

void PersonGroupsDialog::setupTableView()
{
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
}

void PersonGroupsDialog::updateTableView()
{
    //clear
    model->removeRows(0, model->rowCount());
    //display
    ListNodePosi(PersonGroup) pit = this->persongroups->_elem.first();
    for(int i = 0; i < persongroups->count(); ++i, pit = pit->succ)
    {
        model->setItem(i, IDColumn, new QStandardItem(QString::number(pit->data.id)));
        model->item(i, IDColumn)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i, NameColumn, new QStandardItem(QString::fromStdString(pit->data.name)));
        model->item(i, NameColumn)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i, CountColumn, new QStandardItem(QString::number(pit->data.count)));
        model->item(i, CountColumn)->setTextAlignment(Qt::AlignCenter);
    }
    //resize
    ui->tableView->resizeColumnsToContents();
}

void PersonGroupsDialog::onAddNewPersonGroup(PersonGroup *pg)
{
    pg->id = ++this->persongroups->maxID;
    this->persongroups->insertAsLast(*pg);
    delete pg;
    this->updateTableView();
}

void PersonGroupsDialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        int id = this->model->item(index.row(), IDColumn)->text().toInt();
        PersonGroupInfo *dialog = new PersonGroupInfo(&(this->persongroups->findByID(id)->data), this->persongroups, this);
        dialog->exec();
        this->updateTableView();
        delete dialog;
    }
}

void PersonGroupsDialog::on_newPushButton_clicked()
{
    PersonGroupInfo *dialog = new PersonGroupInfo(this->persongroups, this);
    connect(dialog, SIGNAL(addNewPersonGroup(PersonGroup*)), this, SLOT(onAddNewPersonGroup(PersonGroup*)));
    dialog->exec();
    delete dialog;
}

void PersonGroupsDialog::on_delPushButton_clicked()
{
    if(ui->tableView->currentIndex().row() == -1)
        return;
    //Get selected row
    int selectedRow = ui->tableView->currentIndex().row();
    //Get id
    int id = this->model->item(selectedRow, IDColumn)->text().toInt();
    //Get pointer
    ListNodePosi(PersonGroup) g = this->persongroups->findByID(id);
    //Get name
    QString name = this->model->item(selectedRow, NameColumn)->text();
    //Empty
    if(g->data.count == 0)
    {
        if(QMessageBox::information(this, "确认删除", "确认删除分组" + name + "吗？",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            //Remove and update
            this->persongroups->remove(g);
            this->updateTableView();
        }
    }
    //Not Empty
    else
    {
        auto rst = QMessageBox::information(this, "确认删除", "确认删除并清空分组" + name + "吗？选是将删除<strong>并清空分组</strong>，选否将仅删除分组，选取消将不会进行任何更改。",
                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);
        if(rst == QMessageBox::Yes)
        {
            emit(this->clearPersonGroup(&(g->data)));
            this->persongroups->remove(g);
            this->updateTableView();
        }
        else if (rst == QMessageBox::No)
        {
            emit(this->cleanPersonGroup(&(g->data)));
            this->persongroups->remove(g);
            this->updateTableView();
        }
    }
}

void PersonGroupsDialog::on_okPushButton_clicked()
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
    emit(this->selectPersonGroup(&(this->persongroups->findByID(id)->data)));
    this->close();
}

void PersonGroupsDialog::on_cancelPushButton_clicked()
{
    this->close();
}
