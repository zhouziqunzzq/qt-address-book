#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include "iohelper.h"
#include "passwordvalidatedialog.h"
#include "personinfodialog.h"
#include "listNode.h"
#include "person.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    enum{
        IDColumn,
        NameColumn,
        GroupColumn
    };

private:
    Ui::MainWindow *ui;
    IOHelper ioh;
    Persons persons;
    Settings settings;
    PersonGroups persongroups;
    TelephoneGroups telephonegroups;
    EmailGroups emailgroups;
    QStandardItemModel *model;
    void setupModel();
    void setupTableView();

private slots:
    void validatePassword(std::string pwd, PasswordValidateDialog* pd);
    void on_personTableView_doubleClicked(const QModelIndex &index);
    void on_pushButton_5_clicked();
    void onAddnewperson(Person *newPerson);
    void updateTableView();
    void on_newPushButton_clicked();
};

#endif // MAINWINDOW_H
