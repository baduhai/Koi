//
// Created by da-viper on 06/09/2020.
//

#ifndef KOI_PROFILEMANAGER_H
#define KOI_PROFILEMANAGER_H

//Qt
#include <QString>
#include <QDebug>
#include <KSharedConfig>

//koi
#include "profile.h"

class ProfileManager
{
public:
	ProfileManager();
	~ProfileManager();

	static ProfileManager *instance();
	//loads all profile including the defaults.
	QList<const Profile *> allProfiles();

	void saveProfile(const QString& profileName);

	const Profile *defaultProfile() const;

    Profile * _activeProfile ;


	bool isFavourite(const Profile *p );
private:
	static bool profileExists(const QString &fileName, const QFileInfoList &fileList);
	//load the profile to the qhash. hashed with their filename
	void loadProfiles();

	//get the list of profiles
	QStringList listProfiles();

	bool addProfile(const QString &profileName);
	QStringList listFavourites();

	//for storing the loaded profiles.
	QHash<QString, const Profile *> _profileList;
	bool m_loadedAllProfiles;

	//makes sure it is only one instance of the class
	//this is loaded when you use koi for the first time
	// or don't have and profile set
	static const Profile _defaultProfile;
	Q_DISABLE_COPY(ProfileManager);
};


#endif //KOI_PROFILEMANAGER_H
