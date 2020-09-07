//
// Created by da-viper on 04/09/2020.
//

#include "editprofiledialog.h"


EditProfileDialog::EditProfileDialog(QWidget *parent)//cannot pass in a profile here.
    :
	KPageDialog(parent),
	_stylesDialog(nullptr),
	_othersDialog(nullptr),
	_extDialog(nullptr)
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


}
EditProfileDialog::~EditProfileDialog()
{
}
void EditProfileDialog::setProfile(Profile p)
{
	//checks if it points to anything befor using .
	_profile = &p;
	updatePages();
}
void EditProfileDialog::updatePages()
{
	Q_ASSERT(_profile);


}
void EditProfileDialog::setupPage()
{
	//Styles Page.
	_stylesDialog->plasmaComboBox->addItems(Utils::getPlasmaStyles());
	_stylesDialog->QtComboBox->addItems(Utils::getColorSchemes());
	_stylesDialog->gtkComboBox->addItems(Utils::getGtkThemes());
	_stylesDialog->widgetComboBox->addItems(Utils::getWidgetStyles());




}


