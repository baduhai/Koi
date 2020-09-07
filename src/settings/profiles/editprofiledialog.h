//
// Created by da-viper on 04/09/2020.
//

#ifndef EDITPROFILEDIALOG_H
#define EDITPROFILEDIALOG_H


#include <KPageDialog>

#include "ui_externaldialog.h"
#include "ui_stylesdialog.h"
#include "ui_othersdialog.h"

namespace Ui {
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
	
private:
	Ui::StylesDialog *_stylesDialog;
	Ui::OthersDialog *_othersDialog;
    Ui::ExternalDialog *_extDialog;
};


#endif //EDITPROFILEDIALOG_H
