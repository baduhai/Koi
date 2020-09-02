#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

//QT
#include <QDialog>
#include <QModelIndexList>
#include <QListWidgetItem>

//KDE
#include <KPageDialog>

//koi
#include "utils.h"
#include "generalsettings.h"

namespace Ui {
	class SettingDialog;
	class GeneralSettings;
}

class SettingDialog : public KPageDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog() override;

private slots:

private:
//    QMap<KPageWidgetItem *, Page> _pages;

    QDialogButtonBox *_buttonBox;
    Ui::SettingDialog *ui;
    GeneralSettings *_generalUi;

};

#endif // SETTINGDIALOG_H
