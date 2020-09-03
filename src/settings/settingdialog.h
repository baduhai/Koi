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
	class GeneralSettings;
}

class SettingDialog : public KPageDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog() override;

private slots:
	void slotOkClicked();

private:
//    QMap<KPageWidgetItem *, Page> _pages;

    GeneralSettings *_generalUi;
	QSettings *settings ;
};

#endif // SETTINGDIALOG_H
