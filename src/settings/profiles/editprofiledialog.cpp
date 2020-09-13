//
// Created by da-viper on 04/09/2020.
//

#include "editprofiledialog.h"


EditProfileDialog::EditProfileDialog(QWidget *parent)//cannot pass in a profile here.
	:
	KPageDialog(parent),
	_stylesDialog(nullptr),
	_othersDialog(nullptr),
	_extDialog(nullptr),
	_profile(nullptr)
{
	setWindowTitle("Edit"  " profile");
	setFaceType(KPageDialog::Tabbed);
	//Adding Pages
	//Styles page
	const QString stylePageName("Styles");
	_stylesDialog = new Ui::StylesDialog();
	auto *stylePageWidget = new QWidget(this);
	_stylesDialog->setupUi(stylePageWidget);
	//put this to make it easier to read
	this->addPage(stylePageWidget, stylePageName);

	//otherPage
	const QString othersPageName("Other");
	_othersDialog = new Ui::OthersDialog();
	auto *othersPageWidget = new QWidget(this);
	_othersDialog->setupUi(othersPageWidget);
	addPage(othersPageWidget, othersPageName);

	//External Page
	const QString extPageName("External");
	_extDialog = new Ui::ExternalDialog();
	auto *extPageWidget = new QWidget(this);
	_extDialog->setupUi(extPageWidget);
	addPage(extPageWidget, extPageName);




	//update pages
	setupPage();

	connect(this, &EditProfileDialog::accepted, this, &EditProfileDialog::saveProfile);
	//Styles Page
	connect(_stylesDialog->nameCheckBox, &QCheckBox::stateChanged, this, &EditProfileDialog::enableProfileName);
	connect(_stylesDialog->widgetBox, &QComboBox::currentTextChanged, this, &EditProfileDialog::enableKvantum);

	//Others Page.
	connect(_othersDialog->wallpaperCheckBox,
			&QCheckBox::stateChanged,
			_othersDialog->wallpaperBtn,
			&QPushButton::setEnabled);
	connect(_othersDialog->scriptCheckBox,
			&QCheckBox::stateChanged,
			_othersDialog->scriptBtn,
			&QPushButton::setEnabled);
}
EditProfileDialog::~EditProfileDialog()
{
}
void EditProfileDialog::setProfile(Profile *p)
{
	//checks if it points to anything before using .
	_profile = p;
	updatePages();
}
//may seperate this in the future
void EditProfileDialog::updatePages()
{
	Q_ASSERT(_profile);
	//Styles Page.
	//TODO enable later when you can delete profiles
//	if(_profile->name()== "light" || _profile->name() == "dark"){
	_stylesDialog->nameCheckBox->setHidden(true);
//	}
	_stylesDialog->nameTextBox->setText(_profile->name());
	_stylesDialog->plasmaBox->setCurrentText(_profile->getPlasma());
	_stylesDialog->colorBox->setCurrentText(_profile->getColor());
	_stylesDialog->gtkBox->setCurrentText(_profile->getGtk());
	_stylesDialog->widgetBox->setCurrentText(_profile->getWidget());
	_stylesDialog->kvBox->setCurrentText(_profile->getKvantum());
	//Others Page
	_othersDialog->iconBox->setCurrentText(_profile->getIcon());
	_othersDialog->cursorBox->setCurrentText(_profile->getMouse());
}
void EditProfileDialog::setupPage()
{
	//TODO there maybe a  more efficient way to do this.
	//Styles Page.
	_stylesDialog->plasmaBox->addItems(Utils::getPlasmaStyles());
	_stylesDialog->colorBox->addItems(Utils::getColorSchemes());
	_stylesDialog->gtkBox->addItems(Utils::getGtkThemes());
	_stylesDialog->widgetBox->addItems(Utils::getWidgetStyles());
	_stylesDialog->kvBox->addItems(Utils::getKvantumStyles());

	//Others page
	_othersDialog->iconBox->addItems(Utils::getIcons());
	_othersDialog->cursorBox->addItems(Utils::getCursorThemes());

}
void EditProfileDialog::saveProfile()
{
	//TODO save only stuff that has been changed.
	//Style Page
	_profile->setName(_stylesDialog->nameTextBox->text());
	_profile->setPlasma(_stylesDialog->plasmaBox->currentText());
	_profile->setColor(_stylesDialog->colorBox->currentText());
	_profile->setGtk(_stylesDialog->gtkBox->currentText());
	_profile->setWidget(_stylesDialog->widgetBox->currentText());
	_profile->setKvantum(_stylesDialog->kvBox->currentText());

	//Others
	_profile->setIcon(_othersDialog->iconBox->currentText());
	_profile->setMouse(_othersDialog->cursorBox->currentText());

	//this is meant to be in the controller as i currently don't know how.
	ProfileManager::instance()->addProfile(_profile);
	ProfileManager::instance()->saveProfile(_profile->name());

}
void EditProfileDialog::enableKvantum(const QString &widgetName)
{
	if (widgetName == "kvantum" || widgetName == "kvantum-dark") {
		_stylesDialog->kvBox->setEnabled(true);
	}
	else {
		_stylesDialog->kvBox->setEnabled(false);
	}
}
void EditProfileDialog::enableProfileName(const int &state)
{
	_stylesDialog->nameTextBox->setEnabled(state);
}


