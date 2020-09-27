#include "generalsettings.h"


GeneralSettings::GeneralSettings(QWidget *parent)
	:
        QWidget(parent),
        ui(new Ui::GeneralSettings)
{
	ui->setupUi(this);
    loadSettings();
	//connect(ui->notifyCheckBox, &QCheckBox::toggled,this, &GeneralSettings::enableNotification);
}

GeneralSettings::~GeneralSettings()
{
	delete ui;
}
void GeneralSettings::saveSettings()
{
	//notification.
	//by default the general group is created if there is no group declared.
	m_settings.setValue("notify", ui->notifyCheckBox->isChecked());
}
void GeneralSettings::loadSettings()
{
	ui->notifyCheckBox->setChecked(m_settings.value("notify").toBool());
}

