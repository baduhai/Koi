//
// Created by da-viper on 04/09/2020.
//

#ifndef PROFILEDIALOGGROUP_H
#define PROFILEDIALOGGROUP_H


#include <KPageDialog>

class ProfileDialogGroup : public KPageDialog
{
	Q_OBJECT
public:
	explicit ProfileDialogGroup(QWidget *parent = nullptr);
	~ProfileDialogGroup() override ;

};


#endif //PROFILEDIALOGGROUP_H
