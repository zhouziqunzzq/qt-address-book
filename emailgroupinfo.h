#ifndef EMAILGROUPINFO_H
#define EMAILGROUPINFO_H

#include <QDialog>

namespace Ui {
class EmailGroupInfo;
}

class EmailGroupInfo : public QDialog
{
    Q_OBJECT

public:
    explicit EmailGroupInfo(QWidget *parent = 0);
    ~EmailGroupInfo();

private:
    Ui::EmailGroupInfo *ui;
};

#endif // EMAILGROUPINFO_H
