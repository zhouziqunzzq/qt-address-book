#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Settings *s, QWidget *parent = 0);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;
    Settings *settings;

private slots:
    void displayInfo();
    void saveInfo();
    void on_okPushButton_clicked();
    void on_checkBox_clicked();
};

#endif // SETTINGSDIALOG_H
