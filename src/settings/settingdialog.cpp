#include "settingdialog.h"


SettingDialog::SettingDialog(QWidget *parent)
	:
	KPageDialog(parent),
	_generalUi(nullptr),
	_buttonBox(nullptr)
{
	setWindowTitle("Settings Page");
	setFaceType(KPageDialog::List);

	_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	setButtonBox(_buttonBox);

	QPushButton *okButton = _buttonBox->button(QDialogButtonBox::Ok);
	//  okButton->setDefault(true);

	const auto defaultIcon = QIcon::fromTheme(QStringLiteral("utilities-terminal"));

	//adding Pages
	// General page
	const QString generalPageName = ("General");
	_generalUi = new GeneralSettings();

//	this->addPage(_generalUi,generalPageName);
	auto *generalPageItem = this->addPage(_generalUi, generalPageName);
	generalPageItem->setHeader(generalPageName);
	generalPageItem->setIcon(QIcon::fromTheme(QStringLiteral("application")));

	// Profiles Page
	const QString profilePageName = ("Profiles");

}

SettingDialog::~SettingDialog()
{
	delete _buttonBox;
}

