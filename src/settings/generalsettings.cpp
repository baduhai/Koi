#include "generalsettings.h"


GeneralSettings::GeneralSettings(QWidget *parent)
    :
    QWidget(parent),
    ui(new Ui::GeneralSettings)
{
    ui->setupUi(this);
    loadSettings();

    //TODO enable this when you have implemented autostart
    ui->autostartCheckBox->setHidden(true);

    ui->generalMes->setHidden(true);
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
    m_settings.setValue("start-hidden", ui->hiddenCheckBox->isChecked());
}
void GeneralSettings::loadSettings()
{
    ui->notifyCheckBox->setChecked(m_settings.value("notify").toBool());
    ui->hiddenCheckBox->setChecked(m_settings.value("start-hidden").toBool());
}

