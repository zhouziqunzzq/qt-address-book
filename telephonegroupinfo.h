#ifndef TELEPHONEGROUPINFO_H
#define TELEPHONEGROUPINFO_H

#include <QDialog>

namespace Ui {
class TelephoneGroupInfo;
}

class TelephoneGroupInfo : public QDialog
{
    Q_OBJECT

public:
    explicit TelephoneGroupInfo(QWidget *parent = 0);
    ~TelephoneGroupInfo();

private:
    Ui::TelephoneGroupInfo *ui;
};

#endif // TELEPHONEGROUPINFO_H
