#include "generalsettings.h"


GeneralSettings::GeneralSettings(QWidget *parent, QSettings *pSettings)
	:
	QWidget(parent),
	ui(new Ui::GeneralSettings),
	setting(pSettings)
{
	ui->setupUi(this);
	loadSetting();
	//connect(ui->notifyCheckBox, &QCheckBox::toggled,this, &GeneralSettings::enableNotification);
}

GeneralSettings::~GeneralSettings()
{
	delete ui;
}
void GeneralSettings::saveChanges()
{
	//notification.
	//by default the general group is created if there is no group declared.
	setting->setValue("notify", ui->notifyCheckBox->isChecked());
}
void GeneralSettings::loadSetting()
{
	ui->notifyCheckBox->setChecked(setting->value("notify").toBool());
}

