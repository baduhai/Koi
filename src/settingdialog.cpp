#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    //this connects the pagelist items to the stacked widgets
    connect(ui->sPageList, &QListWidget::currentRowChanged,
            ui->stackedWidget, &QStackedWidget::setCurrentIndex);

}

SettingDialog::~SettingDialog()
{
    delete ui;
}


void SettingDialog::on_buttonBox_rejected()
{
    accept();
}

void SettingDialog::on_pushButton_clicked()
{
    ui->sPageList->currentItem()->setBackgroundColor(Qt::red);
}

void SettingDialog::on_sPageList_itemClicked(QListWidgetItem *item)
{

}
