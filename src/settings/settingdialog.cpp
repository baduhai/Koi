/*
 * this is the dialog widget for all settings in  koi
 * inherited from kPagedialog and has three widget item
 * -General , -Profile , and Others
 * */
#include "settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent)
	:
	KPageDialog(parent),
	_generalUi(nullptr),
	_profileUi(nullptr),
	_scheduleUi(nullptr)
{
    this->setMinimumSize(605,353);
	// KOI Settings Location on the drive
	buttonBox()->setStandardButtons(QDialogButtonBox::Ok
										| QDialogButtonBox::Cancel);

	setWindowTitle("Settings Page");
	setFaceType(KPageDialog::List);

	//Adding Pages
	// General page
	const QString generalPageName = ("General");
	_generalUi = new GeneralSettings(this);
	KPageWidgetItem *generalPageItem = this->addPage(_generalUi, generalPageName);
	generalPageItem->setParent(this);
	generalPageItem->setHeader(generalPageName);
	generalPageItem->setIcon(QIcon::fromTheme(QStringLiteral("settings-configure")));

	// Profiles Page
	const QString profilePageName = ("Profiles");
	_profileUi = new ProfileSettingsDialog(this);
	KPageWidgetItem *profilePageItem = addPage(_profileUi, profilePageName);
	profilePageItem->setParent(this);
	profilePageItem->setHeader(profilePageName);
	profilePageItem->setIcon(QIcon::fromTheme("systemsettings"));

	//Schedule Page
	const QString schedulePageName = ("Schedule");
	_scheduleUi = new ScheduleProfile(this);
	KPageWidgetItem *schedulePageItem = addPage(_scheduleUi, schedulePageName);
	schedulePageItem->setParent(this);
	schedulePageItem->setHeader(schedulePageName);
	schedulePageItem->setIcon(QIcon::fromTheme("clock"));

	connect(this, &SettingDialog::accepted, this, &SettingDialog::slotOkClicked);
	connect(buttonBox()->button(QDialogButtonBox::Cancel),
			&QAbstractButton::clicked,
			this,
			&SettingDialog::cancelClicked);
}

SettingDialog::~SettingDialog()
{
	delete _generalUi;
	delete _profileUi;
	delete _scheduleUi;

}
void SettingDialog::slotOkClicked()
{
    _generalUi->saveSettings();
    _scheduleUi->saveSettings();
}
void SettingDialog::cancelClicked()
{
//TODO may implement save changes dialog.
}

