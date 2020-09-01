#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QModelIndexList>
#include <QListWidgetItem>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

private slots:
    void on_buttonBox_rejected();

    void on_pushButton_clicked();

    void on_sPageList_itemClicked(QListWidgetItem *item);

private:
    Ui::SettingDialog *ui;
};

#endif // SETTINGDIALOG_H
