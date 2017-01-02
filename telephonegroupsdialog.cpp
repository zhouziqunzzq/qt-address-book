#include "telephonegroupsdialog.h"
#include "ui_telephonegroupsdialog.h"

TelephoneGroupsDialog::TelephoneGroupsDialog(TelephoneGroups *tg, bool isSelection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelephoneGroupsDialog),
    telephonegroups(tg)
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
    this->model = new QStandardItemModel(this->telephonegroups->count(), 2);
    this->setupModel();
    this->setupTableView();
    this->updateTableView();
}

TelephoneGroupsDialog::~TelephoneGroupsDialog()
{
    delete ui;
}

void TelephoneGroupsDialog::setupModel()
{
    model->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("编号"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("组名"));
}

void TelephoneGroupsDialog::setupTableView()
{
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
}

void TelephoneGroupsDialog::updateTableView()
{
    //clear
    model->removeRows(0, model->rowCount());
    //display
    ListNodePosi(TelephoneGroup) pit = this->telephonegroups->_elem.first();
    for(int i = 0; i < telephonegroups->count(); ++i, pit = pit->succ)
    {
        model->setItem(i, IDColumn, new QStandardItem(QString::number(pit->data.id)));
        model->item(i, IDColumn)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i, NameColumn, new QStandardItem(QString::fromStdString(pit->data.name)));
        model->item(i, NameColumn)->setTextAlignment(Qt::AlignCenter);
    }
    //resize
    //ui->personTableView->resizeColumnsToContents();
}

void TelephoneGroupsDialog::onAddNewTelephoneGroup(TelephoneGroup *tg)
{
    tg->id = ++this->telephonegroups->maxID;
    this->telephonegroups->insertAsLast(*tg);
    delete tg;
    this->updateTableView();
}

void TelephoneGroupsDialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        int id = this->model->item(index.row(), IDColumn)->text().toInt();
        TelephoneGroupInfo *dialog = new TelephoneGroupInfo(&(this->telephonegroups->findByID(id)->data), this->telephonegroups, this);
        dialog->exec();
        this->updateTableView();
    }
}

void TelephoneGroupsDialog::on_newPushButton_clicked()
{
    TelephoneGroupInfo *dialog = new TelephoneGroupInfo(this->telephonegroups, this);
    connect(dialog, SIGNAL(addNewTelephoneGroup(TelephoneGroup*)), this, SLOT(onAddNewTelephoneGroup(TelephoneGroup*)));
    dialog->exec();
}

void TelephoneGroupsDialog::on_delPushButton_clicked()
{
    if(ui->tableView->currentIndex().row() == -1)
        return;
    //Get selected row
    int selectedRow = ui->tableView->currentIndex().row();
    //Get id
    int id = this->model->item(selectedRow, IDColumn)->text().toInt();
    //Get pointer
    ListNodePosi(TelephoneGroup) g = this->telephonegroups->findByID(id);
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
            emit(this->cleanTelephoneGroup(&(g->data)));
        this->telephonegroups->remove(g);
        this->updateTableView();
    }
}

void TelephoneGroupsDialog::on_okPushButton_clicked()
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
    emit(this->selectTelephoneGroup(&(this->telephonegroups->findByID(id)->data)));
    this->close();
}

void TelephoneGroupsDialog::on_cancelPushButton_clicked()
{
    this->close();
}
