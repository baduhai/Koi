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
Q_GLOBAL_STATIC(ProfileManager,theProfileManager);
ProfileManager *ProfileManager::instance()
{
	return theProfileManager;
}

QFileInfoList ProfileManager::listProfiles()
{
	QFileInfoList pList;
	QDir dirs(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/koi"));
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

void ProfileManager::loadProfiles()
{
	const auto localProfilesList = listProfiles();

	for (const auto &localProfiles : localProfilesList) {
		if (!loadProfile(localProfiles)) {
			qDebug() << "failed to load " << localProfiles;
		}
	}

	m_haveLoadedAll = true;

}
bool ProfileManager::loadProfile(const QFileInfo &file)
{
	//checks if it is a .koi file and it exists
	auto filePath = file.absolutePath();

	if (!(file.completeSuffix() != "koi") || !QFileInfo::exists(filePath)) {
		return false;
	}
	auto fileName = file.fileName();

	auto *settings = new QSettings(filePath , QSettings::IniFormat);
	auto p = new Profile();

	p->setName(fileName);
	//can just pass the pointer and remove the reference;
	p->writeConfig(settings);

	if (p->name().isEmpty()) {
                              qDebug()<< "does not have a valid name and was not loaded.";
		delete settings;
        return false;
    }

    if (!_profileList.contains(fileName)) {
        _profileList.insert(p->name(), p);
    } else {
        delete settings;
        qDebug()<< "profile already exists in the list ";
    }
    return true;
}

const Profile ProfileManager::_defaultProfile;
const Profile *ProfileManager::defaultProfile() const
{
	return &_defaultProfile;
}
