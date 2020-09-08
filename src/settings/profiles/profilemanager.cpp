//
// Created by da-viper on 06/09/2020.
//

#include "profilemanager.h"

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

QStringList ProfileManager::listProfiles()
{
	QStringList pList;
	QDir dirs(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
	if (!dirs.exists()) {
		QDir().mkdir(dirs.absolutePath());
		return QStringList(); //empty list.
		qDebug() << "no profiles in koi ";
	}
	pList.reserve(dirs.Size);

	const QStringList fileNames = QDir(dirs).entryList(QStringList() << QStringLiteral("*.koi"));
	for (const auto &file: fileNames) {
		//remove the extension
		pList.append(file.left(file.lastIndexOf(".")));
	}
    return pList;
}
bool ProfileManager::profileExists(const QString &fileName, const QFileInfoList &fileList)
{
	for (const auto &file: fileList) {
		if (QString::compare(fileName, file.baseName(), Qt::CaseInsensitive) == 0) {
			return true;
		}
	}
	return false;
}

void ProfileManager::loadProfiles()
{
	const auto localProfilesList = listProfiles();

	for (const auto &localProfiles : localProfilesList) {
		if (!addProfile(localProfiles)) {
			//qt 5.9.5 does does not support appending qfileinfo
			//in qdebug();
			qDebug() << "failed to load " ;
		}
	}

	m_loadedAllProfiles = true;

}
bool ProfileManager::addProfile(const QString &profileName)
{
	QFileInfo profileInfo(QStandardPaths::writableLocation(
			QStandardPaths::AppLocalDataLocation) + "/" + profileName + ".koi");
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
	auto fileName = profileInfo.baseName();

	auto *settings = new QSettings(filePath, QSettings::IniFormat);
	auto p = new Profile();

	p->setName(fileName);
	//can just pass the pointer and remove the reference;
	p->readConfig(settings);

	if (p->name().isEmpty()) {
		qDebug() << "does not have a valid name and was not loaded.";
		delete settings;
		return false;
	}

	if (!_profileList.contains(fileName)) {
		_profileList.insert(p->name(), p);
	}
	else {
		delete settings;
		qDebug() << "profile already exists in the list ";
	}
	return true;
}

const Profile ProfileManager::_defaultProfile;

const Profile *ProfileManager::defaultProfile() const
{
	return &_defaultProfile;
}
QList<const Profile *> ProfileManager::allProfiles()
{
	QStringList pList(listProfiles());
	if (!m_loadedAllProfiles) {
		loadProfiles();
	}
	//there should be a better way to do this if iam wrong
	//this creates default light and dark profile if there are none.
	QString dark("dark");
	if(!pList.contains(dark)){
		addProfile(dark);
		saveProfile(dark);
	}

	QString light(QStringLiteral("light"));
	if (!pList.contains(light)) {
		addProfile(light);
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


QStringList ProfileManager::listFavourites()
{
	QSettings s(QDir::homePath() + "/.config/koirc", QSettings::IniFormat);
	return  s.value("favourites").toStringList();
}

void ProfileManager::saveProfile(const QString &profileName)
{
	QFileInfo profileInfo(QStandardPaths::writableLocation(
			QStandardPaths::AppLocalDataLocation) + "/" + profileName + ".koi");
	QSettings s(profileInfo.absoluteFilePath(), QSettings::IniFormat);
	_profileList.value(profileName)->writeConfig(&s);
	_profileList.value(profileName)->createProfileGlobalDir();
	_profileList.value(profileName)->writeToGlobal();

}
