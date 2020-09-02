#include "generalsettings.h"


GeneralSettings::GeneralSettings(QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::GeneralSettings)
{
	ui->setupUi(this);
	connect(ui->notifyCheckBox, &QCheckBox::toggled,this, &GeneralSettings::enableNotification);
}

GeneralSettings::~GeneralSettings()
{
	delete ui;
}
void GeneralSettings::enableNotification(bool enable)
{
	qDebug() << "this is " << enable;
	//utils.settings->setValue("General/notify", enable);
}

