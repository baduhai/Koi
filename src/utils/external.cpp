//
// Created by da-viper on 17/10/2020.
//

#include "external.h"

namespace utils
{

QStringList getKonsoleThemes()
{
    QStringList kThemes;
    QFileInfoList kDirPaths(noUse::getKonsoleDirPaths());
    for (const auto &path:kDirPaths) {
        if (path.exists()) {
            kThemes.append(path.baseName());
        }
    }
    kThemes.sort();
    return kThemes;
}

namespace noUse
{
void setKonsoleTheme(const QString &name)
{
    QFileInfoList profileList;
    QSettings s(QDir::homePath() + QStringLiteral("/.config/konsolerc"), QSettings::IniFormat);
    QString kProfileName(s.value(QStringLiteral("Desktop Entry/DefaultProfile")).toString());
    auto kProfilePaths(QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
                                                 QStringLiteral("konsole"),
                                                 QStandardPaths::LocateDirectory));
    for (const auto &path: kProfilePaths) {
        profileList.append(QDir(path).entryInfoList(QStringList({QStringLiteral("*.profile")}), QDir::Files));
    }
    for (const auto &profileInfo : profileList) {
        if (profileInfo.fileName() == kProfileName) {
            QSettings cs(profileInfo.absoluteFilePath(), QSettings::IniFormat);
            cs.setValue("Appearance/ColorScheme", name);
        }
    }

}

QFileInfoList getKonsoleDirPaths()
{
    static QFileInfoList dirPathList;
    const QStringList kDir(QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
                                                     QStringLiteral("konsole"),
                                                     QStandardPaths::LocateDirectory));
    QStringList kThemes;
    for (const auto &dir : kDir) {
        dirPathList.append(QDir(dir).entryInfoList(QStringList({QStringLiteral("*.colorscheme")}), QDir::Files));
    }
    return dirPathList;
}
}
}
