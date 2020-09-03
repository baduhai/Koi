#include "settingdialog.h"


SettingDialog::SettingDialog(QWidget *parent)
	:
	KPageDialog(parent),
	_generalUi(nullptr)
{
	//setting location
	settings = new QSettings (QDir::homePath() + "/.config/koirc", QSettings::NativeFormat);

	setWindowTitle("Settings Page");
	setFaceType(KPageDialog::List);

	//adding Pages
	// General page
	const QString generalPageName = ("General");
	_generalUi = new GeneralSettings(this, settings);
	KPageWidgetItem *generalPageItem = this->addPage(_generalUi, generalPageName);
	generalPageItem->setHeader(generalPageName);
	generalPageItem->setIcon(QIcon::fromTheme(QStringLiteral("application")));

	// Profiles Page
	const QString profilePageName = ("Profiles");

	connect(this, &SettingDialog::accepted, this, &SettingDialog::slotOkClicked);
}

SettingDialog::~SettingDialog()
{
	delete _generalUi;
	delete settings;
}
void SettingDialog::slotOkClicked()
{
	_generalUi->applySetting();


}

