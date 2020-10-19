//
// Created by da-viper on 06/09/2020.
//

#include "profilemanager.h"
#include <QLoggingCategory>

ProfileManager::ProfileManager()
	: _profileList(),
	m_favourites()
{
	loadFavourites();
	loadProfiles();
}

ProfileManager::~ProfileManager() = default;

//see https://doc.qt.io/qt-5/qglobalstatic.html#Q_GLOBAL_STATIC
Q_GLOBAL_STATIC(ProfileManager, theProfileManager);
ProfileManager *ProfileManager::instance()
{
	return theProfileManager;
}

QFileInfoList ProfileManager::listProfiles()
{
	QFileInfoList pList;
    QDir dirs(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/koi");
	if (!dirs.exists()) {
		QDir().mkdir(dirs.absolutePath());
		qDebug() << "no profiles in koi ";
		return QFileInfoList(); //empty list.
	}
	pList.reserve(dirs.Size);

	pList = QDir(dirs).entryInfoList(QStringList() << QStringLiteral("*.koi"));
	return pList;
}

bool ProfileManager::profileExists(const QString &fileName)
{
	if (_profileList.contains(fileName)) {
		return true;
	}
	return false;
}

void ProfileManager::loadProfiles()
{
	const auto localProfilesList = listProfiles();

	for (const auto &localProfileName : localProfilesList) {
		QString path(localProfileName.filePath());
		QSettings settings(path, QSettings::NativeFormat);

		//TODO fix memory leak here
		QPointer<Profile> localProfile = new Profile;
		localProfile->setParent(this);
		localProfile->setName(localProfileName.baseName());
		localProfile->setGlobDir();
		localProfile->setConfigPath();
		localProfile->readConfig(settings);

		if (!addProfile(localProfile)) {
			//qt 5.9.5 does does not support appending qfileinfo
			//in qdebug();
			qDebug() << "failed to load " << localProfile->name();
		}
	}

	m_loadedAllProfiles = true;

}
//this would add the profile to the hash table.
bool ProfileManager::addProfile(Profile *profile)
{
	QFileInfo profileInfo(profile->configPath());

	//checks if it is a .koi file and it exists
	auto filePath = profileInfo.absoluteFilePath();

	if (profileInfo.completeSuffix() != "koi") {
		return false;
	}

	if (!profileInfo.exists()) {
		QFile f(filePath);
		if (f.open(QIODevice::ReadWrite)) {
			qDebug() << "file now exists";
		}
	}

	if (!profileExists(profile->name())) {
		_profileList.insert(profile->name(), profile);
	}
	else {
		qDebug() << "Profile already exist use a different Name for it ";
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
	QStringList defProfileList({"dark","light"});
	for(const auto &defProfileName: qAsConst(defProfileList)){
	    if (!profileExists(defProfileName)){
	        QFileInfo defProfileInfo(QDir::homePath() + "/.local/koi/" + defProfileName + ".koi");
	        QSettings settings(defProfileInfo.absoluteFilePath(), QSettings::IniFormat);
	        auto *dProfile = new Profile;
            dProfile->setName(defProfileName);
            dProfile->setGlobDir();
            dProfile->setConfigPath();
            if (!addProfile(dProfile)) {
                //qt 5.9.5 does does not support appending qfileinfo in qdebug();
                qDebug() << "failed to load ";
            }
            saveProfile(defProfileName);
	    }
	}
	//gotten profiles from disk.
	return _profileList.values();
}

bool ProfileManager::isFavourite(const QString &profileName)
{
	return m_favourites.contains(profileName);
}

Profile *ProfileManager::getProfile(const QString &profileName)
{
	return _profileList.value(profileName);
}

QHash<QString, QString> ProfileManager::getFavouritesList() const
{
	return m_favourites;
}

void ProfileManager::loadFavourites()
{
	QSettings s;
	s.beginGroup("Favourites");
	QStringList keys(s.allKeys());
	for (const auto &key : keys) {
		if (!m_favourites.contains(key)) {
			m_favourites.insert(key, s.value(key).toString());
		}
	}
	if (!m_favourites.contains("light")) {
		m_favourites.insert("light", QString());
	}
	if (!m_favourites.contains("dark")) {
		m_favourites.insert("dark", QString());
	}
}

void ProfileManager::saveProfile(const QString &profileName)
{
	Q_ASSERT(_profileList.value(profileName));
	auto profile = _profileList.value(profileName);
	QFileInfo profileInfo(profile->configPath());
	QSettings s(profileInfo.absoluteFilePath(), QSettings::IniFormat);
	profile->writeConfig(s);
	profile->createProfileGlobalDir();
	profile->writeToGlobal();
}
void ProfileManager::deleteProfile()
{
	Q_ASSERT(_activeProfile);
	//TODO add more safety traps here
	QDir delProfile(_activeProfile->getGlobDir());
	if (delProfile.exists()) {
		qDebug() << "deleting dir" << _activeProfile->getGlobDir();
		delProfile.removeRecursively();
	}
	if (QFile::exists(_activeProfile->configPath())) {
		qDebug() << "Deleting " << _activeProfile->configPath();
		QFile::remove(_activeProfile->configPath());
	}
}
void ProfileManager::setFavourite(const QString& profileName, bool favourite)
{
	if (favourite && !m_favourites.contains(profileName)) {
		m_favourites.insert(profileName, QString());
		emit favouritesChanged(profileName,favourite);
	}
	else if (!favourite && m_favourites.contains(profileName)) {
		m_favourites.remove(profileName);
		emit favouritesChanged(profileName, favourite);
	}
}

void ProfileManager::saveFavourites()
{
	QSettings s;
	s.beginGroup("Favourites");
	s.remove("");
	QHashIterator<QString, QString> i(m_favourites);
	while (i.hasNext()) {
		i.next();
		s.setValue(i.key(), i.value());
	}
	s.endGroup();
	s.sync();
}
QStringList ProfileManager::favouriteNames() const
{
    QStringList favs;
    QHashIterator<QString, QString> i(m_favourites);
	while (i.hasNext()) {
		i.next();
		favs.append(i.key());
	}
    return favs;
}
