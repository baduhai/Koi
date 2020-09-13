    //
// Created by da-viper on 06/09/2020.
//

#include "profilemanager.h"
#include <QLoggingCategory>

ProfileManager::ProfileManager()
{
}

ProfileManager::~ProfileManager()
{
}
//see https://doc.qt.io/qt-5/qglobalstatic.html#Q_GLOBAL_STATIC
Q_GLOBAL_STATIC(ProfileManager, theProfileManager);
ProfileManager *ProfileManager::instance()
{
	return theProfileManager;
}

QHash<QString, Profile *> ProfileManager::_profileList{} ;
Profile* ProfileManager::_activeProfile {};

QFileInfoList ProfileManager::listProfiles()
{
	QFileInfoList pList;
	QDir dirs(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
	if (!dirs.exists()) {
		QDir().mkdir(dirs.absolutePath());
		qDebug() << "no profiles in koi ";
		return QFileInfoList(); //empty list.
	}
	pList.reserve(dirs.Size);

	pList = QDir(dirs).entryInfoList(QStringList() << QStringLiteral("*.koi"));
    return pList;
}

bool ProfileManager::profileExists(const QString &fileName, const QHash<QString,Profile *> &profileList)
{
	if(profileList.contains(fileName)){
		return true;
	}
	return false;
}

void ProfileManager::loadProfiles()
{
	const auto localProfilesList = listProfiles();

	for (const auto &localProfileName : localProfilesList) {
        QString path(localProfileName.filePath());
        QSettings settings(path , QSettings::NativeFormat);

        auto *localProfile = new Profile;
		localProfile->setName(localProfileName.baseName());
		localProfile->readConfig(settings);

		if (!addProfile(localProfile)) {
			//qt 5.9.5 does does not support appending qfileinfo
			//in qdebug();
			qDebug() << "failed to load " ;
		}
	}

	m_loadedAllProfiles = true;

}
//this would add the profile to the hash table.
bool ProfileManager::addProfile(Profile *profile)
{
	QFileInfo profileInfo(QStandardPaths::writableLocation(
			QStandardPaths::AppLocalDataLocation) + "/" + profile->name() + ".koi");
	//checks if it is a .koi file and it exists
	auto filePath = profileInfo.absoluteFilePath();

	if (profileInfo.completeSuffix() != "koi") {
		return false;
	}

	if (!QFileInfo::exists(filePath)) {
		QFile f(filePath);
		if (f.open(QIODevice::ReadWrite)) {
			qDebug() << "file now exists";
		}
	}

	if (!profileExists(profile->name(),_profileList)){
		_profileList.insert(profile->name(),profile);
	}else{
		qDebug() << "Profile already exist use a different Name for it " ;
	}
	return true;
}

const Profile ProfileManager::_defaultProfile;

const Profile *ProfileManager::defaultProfile() const
{
	return &_defaultProfile;
}
QList<Profile *> ProfileManager::allProfiles()
{
	if (!m_loadedAllProfiles) {
		loadProfiles();
	}
	//there should be a better way to do this if iam wrong
	//this creates default light and dark profile if there are none.
	//Defaults
	QString dark("dark");
	if(!profileExists(dark,_profileList)){
		QFileInfo darkInfo(QDir::homePath() + "/.local/koi/dark.koi");
		QSettings settings(darkInfo.absoluteFilePath() , QSettings::IniFormat);
		auto *dProfile = new Profile;
		dProfile->setName(dark);
		if (!addProfile(dProfile)) {
			//qt 5.9.5 does does not support appending qfileinfo in qdebug();
			qDebug() << "failed to load " ;
		}
		saveProfile(dark);
	}

	QString light(QStringLiteral("light"));
	if(!profileExists(light,_profileList)){
		QFileInfo lightInfo(QDir::homePath() + "/.local/koi/light.koi" );
		QSettings settings(lightInfo.absoluteFilePath() , QSettings::IniFormat);
		auto *lProfile = new Profile;
		lProfile->setName(light);
		if (!addProfile(lProfile)) {
			//qt 5.9.5 does does not support appending qfileinfo in qdebug();
			qDebug() << "failed to load " ;
		}
		saveProfile(light);
	}
	//gotten profiles from disk.
	return _profileList.values();
}

bool ProfileManager::isFavourite(const Profile *p)
{
	QStringList favourites(listFavourites());
	if(favourites.contains(p->name())){
		return true;
	}
    return false;
}

Profile * ProfileManager::getProfile(const QString &profileName)
{
    return _profileList.value(profileName);
}

QStringList ProfileManager::listFavourites()
{
	QSettings s(QDir::homePath() + "/.config/koirc", QSettings::IniFormat);
	return  s.value("favourites").toStringList();
}

void ProfileManager::saveProfile(const QString &profileName)
{
	Q_ASSERT(_profileList.value(profileName));

	QFileInfo profileInfo(QStandardPaths::writableLocation(
			QStandardPaths::AppLocalDataLocation) + "/" + profileName + ".koi");
	QSettings s(profileInfo.absoluteFilePath(), QSettings::IniFormat);
	_profileList.value(profileName)->writeConfig(s);
	_profileList.value(profileName)->createProfileGlobalDir();
	_profileList.value(profileName)->writeToGlobal();

}
