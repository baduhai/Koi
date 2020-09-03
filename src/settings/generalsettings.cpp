#include "generalsettings.h"


GeneralSettings::GeneralSettings(QWidget *parent, QSettings *pSettings)
	:
	QWidget(parent),
	ui(new Ui::GeneralSettings),
	setting(pSettings)
{
	ui->setupUi(this);
	//connect(ui->notifyCheckBox, &QCheckBox::toggled,this, &GeneralSettings::enableNotification);
}

GeneralSettings::~GeneralSettings()
{
	delete ui;
}
void GeneralSettings::applySetting()
{
	//notification.
	qDebug()<< "The old " << setting->value("General/notify").toString();
	setting->setValue("General/notify", ui->notifyCheckBox->isChecked());
	qDebug()<< setting->value("General/notify").toString();
}

