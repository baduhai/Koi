/*
 * this is the dialog widget for all settings in  koi
 * inherited from kPagedialog and has three widget item
 * -General , -Profile , and Others
 * */
#include "settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent)
	:
	KPageDialog(parent),
	_generalUi(new GeneralSettings(this)),
	_profileUi(new ProfileSettingsDialog(this)),
	_scheduleUi(new ScheduleProfile(this))
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
	KPageWidgetItem *generalPageItem = this->addPage(_generalUi, generalPageName);
	generalPageItem->setParent(this);
	generalPageItem->setHeader(generalPageName);
	generalPageItem->setIcon(QIcon::fromTheme(QStringLiteral("settings-configure")));

	// Profiles Page
	const QString profilePageName = ("Profiles");
	KPageWidgetItem *profilePageItem = addPage(_profileUi, profilePageName);
	profilePageItem->setParent(this);
	profilePageItem->setHeader(profilePageName);
	profilePageItem->setIcon(QIcon::fromTheme("systemsettings"));

	//Schedule Page
	const QString schedulePageName = ("Schedule");
	KPageWidgetItem *schedulePageItem = addPage(_scheduleUi, schedulePageName);
	schedulePageItem->setParent(this);
	schedulePageItem->setHeader(schedulePageName);
	schedulePageItem->setIcon(QIcon::fromTheme("clock"));

	connect(this, &SettingDialog::accepted, this, &SettingDialog::slotOkClicked);
	connect(buttonBox()->button(QDialogButtonBox::Cancel),
			&QAbstractButton::clicked,
			this,
			&SettingDialog::cancelClicked);
	//hide the dialog
	connect(_profileUi, &ProfileSettingsDialog::hideSettingsDialog, this , &SettingDialog::setHidden);
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

