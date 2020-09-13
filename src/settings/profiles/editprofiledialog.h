//
// Created by da-viper on 04/09/2020.
//

#ifndef EDITPROFILEDIALOG_H
#define EDITPROFILEDIALOG_H


#include <KPageDialog>

#include "ui_externaldialog.h"
#include "ui_stylesdialog.h"
#include "ui_othersdialog.h"
#include "profile.h"
#include "utils.h"
#include "profilemanager.h"

namespace Ui
{
class OthersDialog;
class StylesDialog;
class ExternalDialog;
}
class EditProfileDialog: public KPageDialog
{
Q_OBJECT
public:
	explicit EditProfileDialog(QWidget *parent = nullptr);
	~EditProfileDialog() override;

	void setupPage();
	void setProfile(Profile *p);
	void updatePages();

	void saveProfile();
	void enableKvantum(const QString &widgetName);
	void enableProfileName(const int &state);

private:
	Ui::StylesDialog *_stylesDialog;
	Ui::OthersDialog *_othersDialog;
	Ui::ExternalDialog *_extDialog;

	//sProfile m_tempProfile;
	Profile *_profile;
};


#endif //EDITPROFILEDIALOG_H
