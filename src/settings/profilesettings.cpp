//
// Created by da-viper on 03/09/2020.
//
/* it inherits the kpagewidge this is where all the profiles are edited and added
 * it has three widget item
 * -Styles, -Others, -External.
 */
#include "profilesettings.h"

ProfileSettings::ProfileSettings(QWidget *parent, QSettings *pSettings)
	:
	QWidget(parent),
	ui(new Ui::ProfileSettings),
	settings(pSettings)
{
	ui->setupUi(this);
	loadDefaults();
}

ProfileSettings::~ProfileSettings() noexcept
{
	delete ui;
}
void ProfileSettings::loadDefaults()
{
}

