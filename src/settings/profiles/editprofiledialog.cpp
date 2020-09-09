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
    setWindowTitle("Profiles Page");
    setFaceType(KPageDialog::List);
    //Adding Pages
    //Styles page
    const QString stylePageName("Styles");
	_stylesDialog = new Ui::StylesDialog() ;
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
	_extDialog = new Ui::ExternalDialog() ;
	auto *extPageWidget = new QWidget(this);
	_extDialog->setupUi(extPageWidget);
	addPage(extPageWidget, extPageName);


	//update pages
	setupPage();

	connect(this , &EditProfileDialog::accepted , this , &EditProfileDialog::saveProfile);

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
    _stylesDialog->plasmaComboBox->setCurrentText(_profile->getPlasma());
    _stylesDialog->colorComboBox->setCurrentText(_profile->getColor());
    _stylesDialog->gtkComboBox->setCurrentText(_profile->getGtk());
	_stylesDialog->widgetComboBox->setCurrentText(_profile->getWidget());
}
void EditProfileDialog::setupPage()
{
	//Styles Page.
	_stylesDialog->plasmaComboBox->addItems(Utils::getPlasmaStyles());
	_stylesDialog->colorComboBox->addItems(Utils::getColorSchemes());
	_stylesDialog->gtkComboBox->addItems(Utils::getGtkThemes());
	_stylesDialog->widgetComboBox->addItems(Utils::getWidgetStyles());

}
void EditProfileDialog::saveProfile()
{
   _profile->setName(_stylesDialog->nameTextBox->text());
   _profile->setPlasma ( _stylesDialog->plasmaComboBox->currentText());
   _profile->setColor(_stylesDialog->colorComboBox->currentText());
   _profile->setGtk(_stylesDialog->plasmaComboBox->currentText() );
   _profile->setWidget(_stylesDialog->widgetComboBox->currentText());

   //this is meant to be in the controller as i currently don't know how.
   ProfileManager::instance()->addProfile(_profile);
   ProfileManager::instance()->saveProfile(_profile->name());

}


