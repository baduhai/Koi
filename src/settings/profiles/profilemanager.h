//
// Created by da-viper on 06/09/2020.
//

#ifndef KOI_PROFILEMANAGER_H
#define KOI_PROFILEMANAGER_H

//Qt
#include <QString>
#include <QDebug>

//koi
#include "profile.h"

class ProfileManager
{
	ProfileManager();
	~ProfileManager();
private:

	//load the profile to the qhash. hashed with their filename
	void loadProfiles();
	//get the list of profiles
	QFileInfoList listProfiles();

	bool loadProfile(const QFileInfo &file);

	bool m_haveLoadedAll;
	//for storing the loaded profiles.
	QHash<QString, const Profile*>_profileList;

	static const Profile _activeProfile;

	//this is loaded when you use koi for the first time
	// or don't have and profile set
	static const Profile _defaultProfile;

	//makes sure it is only one instance of the class
	Q_DISABLE_COPY(ProfileManager);
};


#endif //KOI_PROFILEMANAGER_H
