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
	// KOI Settings Location on the drive
	settings = new QSettings(QDir::homePath() + "/.config/koirc", QSettings::NativeFormat);
	buttonBox()->setStandardButtons(QDialogButtonBox::Ok
										| QDialogButtonBox::Cancel);

	setWindowTitle("Settings Page");
	setFaceType(KPageDialog::List);

	//Adding Pages
	// General page
	const QString generalPageName = ("General");
	_generalUi = new GeneralSettings(this, settings);
	KPageWidgetItem *generalPageItem = this->addPage(_generalUi, generalPageName);
	generalPageItem->setHeader(generalPageName);
	generalPageItem->setIcon(QIcon::fromTheme(QStringLiteral("settings-configure")));

	// Profiles Page
	const QString profilePageName = ("Profiles");
	_profileUi = new ProfileSettingsDialog(this, settings);
	KPageWidgetItem *profilePageItem = addPage(_profileUi, profilePageName);
	profilePageItem->setHeader(profilePageName);
	profilePageItem->setIcon(QIcon::fromTheme("systemsettings"));

	//Schedule Page
	const QString schedulePageName = ("Schedule");
	_scheduleUi = new ScheduleProfile(this);
	KPageWidgetItem *schedulePageItem = addPage(_scheduleUi, schedulePageName);
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
	delete settings;
}
void SettingDialog::slotOkClicked()
{
	_generalUi->saveChanges();
	_scheduleUi->saveChanges();
}
void SettingDialog::cancelClicked()
{
//TODO may implement save changes dialog.
}

