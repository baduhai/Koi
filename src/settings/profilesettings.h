//
// Created by da-viper on 03/09/2020.
//

#ifndef PROFILESETTINGS_H
#define PROFILESETTINGS_H

#include <QSettings>
#include <QWidget>

#include "ui_profilesettings.h"
namespace Ui
{
class ProfileSettings;
}
class ProfileSettings: public QWidget
{
Q_OBJECT
public:
	ProfileSettings(QWidget *parent, QSettings *pSettings);
	~ProfileSettings() override;

	void loadDefaults();

private:
	Ui::ProfileSettings *ui;
	QSettings *settings;

};




#endif //PROFILESETTINGS_H
