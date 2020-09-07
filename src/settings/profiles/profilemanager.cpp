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

QFileInfoList ProfileManager::listProfiles()
{
	QFileInfoList pList;
	QDir dirs(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
	if (!dirs.exists()) {
		QDir().mkdir(dirs.absolutePath());
		return QFileInfoList(); //empty list.
		qDebug() << "no profiles in koi ";
	}
	pList.reserve(dirs.Size);

	const QFileInfoList fileNames = QDir(dirs).entryInfoList(QStringList() << QStringLiteral("*.koi"));
	for (const auto &file: fileNames) {
		pList.append(file);
	}
	return fileNames;
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
		if (!loadProfile(localProfiles)) {
			qDebug() << "failed to load " << localProfiles;
		}
	}

	m_loadedAllProfiles = true;

}
bool ProfileManager::loadProfile(const QFileInfo &file)
{
	//checks if it is a .koi file and it exists
	auto filePath = file.absoluteFilePath();

	if (file.completeSuffix() != "koi") {
		return false;
	}

	if (!QFileInfo::exists(filePath)) {
		QFile f(filePath);
		if (f.open(QIODevice::ReadWrite)) {
			qDebug() << "file now exists";
		}
	}
	auto fileName = file.baseName();

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
	QFileInfoList pList(listProfiles());
	if (!m_loadedAllProfiles) {
		loadProfiles();
	}
	//there should be a better way to do this if iam wrong
	//this creates default light and dark profile if there are none.
	QString dark("dark");
	if (!profileExists(dark, pList)) {
		QFileInfo darkFileInfo(QStandardPaths::writableLocation(
			QStandardPaths::AppLocalDataLocation) + "/" + dark + ".koi");
		loadProfile(darkFileInfo);
		if (!Profile::globalDefaultExists("Koi-" + dark)) {
			QSettings s(darkFileInfo.absoluteFilePath(), QSettings::IniFormat);
			ProfileManager::instance()->_profileList.value(dark)->writeConfig(&s);
			ProfileManager::instance()->_profileList.value(dark)->createProfileGlobalDir();
			ProfileManager::instance()->_profileList.value(dark)->writeToGlobal();
		}
	}

	QString light(QStringLiteral("light"));
	if (!profileExists(light, pList)) {
		QFileInfo lightFileInfo(QStandardPaths::writableLocation(
			QStandardPaths::AppLocalDataLocation) + "/" + light + ".koi");
		loadProfile(lightFileInfo);
		if (!Profile::globalDefaultExists("Koi-" + dark)) {
			QSettings s(lightFileInfo.absoluteFilePath(), QSettings::IniFormat);
			ProfileManager::instance()->_profileList.value(light)->writeConfig(&s);
			ProfileManager::instance()->_profileList.value(light)->createProfileGlobalDir();
			ProfileManager::instance()->_profileList.value(light)->writeToGlobal();
		}
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
