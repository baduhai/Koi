//
// Created by da-viper on 04/09/2020.
//

#include "editprofiledialog.h"


EditProfileDialog::EditProfileDialog(QWidget *parent)
    :
	KPageDialog(parent),
	_stylesDialog(nullptr),
	_othersDialog(nullptr),
	_extDialog(nullptr)
{
    setWindowTitle("Profiles Page");
    setFaceType(KPageDialog::Tabbed);

    //Adding Pages
    //Styles page
    const QString stylePageName("Styles");
	_stylesDialog = new Ui::StylesDialog() ;
	auto *stylePageWidget = new QWidget(this);
	_stylesDialog->setupUi(stylePageWidget);
	auto *stylePageItem = this->addPage(stylePageWidget, stylePageName);

	//otherPage
    const QString othersPageName("Other");
	_othersDialog = new Ui::OthersDialog();
	auto *othersPageWidget = new QWidget(this);
	_othersDialog->setupUi(othersPageWidget);
	auto *othersPageItem = this->addPage(othersPageWidget, othersPageName);

	//External Page
	const QString extPageName("External");
	_extDialog = new Ui::ExternalDialog() ;
	auto *extPageWidget = new QWidget(this);
	_extDialog->setupUi(extPageWidget);
	auto *extPageItem = this->addPage(extPageWidget, extPageName);

}
EditProfileDialog::~EditProfileDialog()
{
}
