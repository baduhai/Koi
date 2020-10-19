//
// Created by da-viper on 16/10/2020.
//

#include "others.h"

namespace utils
{

//Icons
//TODO some of this are not actually icon themes like hicolor /remove them
QStringList getIcons() // Get all available icon themes
{
    QStringList iconDirList(QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
                                                      "icons", QStandardPaths::LocateDirectory));
    QStringList iconThemes;

    for (const auto &path: iconDirList) {
        QDir iconDir(path);
        if (!iconDir.exists()) {
            continue;
        }
        auto themeList(iconDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));
        for (const auto &tName: themeList) {
            QDir themeDir(tName.absoluteFilePath());
            if (themeDir.entryList(QDir::Files | QDir::NoDotAndDotDot).contains(QStringLiteral("index.theme"))) {
                iconThemes.append(themeDir.dirName());
            }
        }
    }
    //TODO a better way
    iconThemes.removeAll(QStringLiteral("hicolor"));
    iconThemes.removeAll(QStringLiteral("default"));
    iconThemes.sort();
    return iconThemes;
}

//cursor
QStringList getCursorThemes()
{
    QStringList cursorThemes;
    QString cursorLocalDir(QDir::homePath() + QStringLiteral("/.icons"));
    //all the possible path of cursors.
    QStringList cursorDirList(QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
                                                        "icons", QStandardPaths::LocateDirectory));
    cursorDirList.append(cursorLocalDir);
    for (const auto &path: cursorDirList) {
        QDir cursorDir(path);
        if (!cursorDir.exists()) {
            continue;
        }
        auto cursorInfoList(cursorDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));
        for (const auto &tName: cursorInfoList) {
            QDir themeDir(tName.absoluteFilePath());
            if (themeDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).contains(QStringLiteral("cursors"))) {
                cursorThemes.append(themeDir.dirName());
            }
        }
    }

    cursorThemes.sort();
    return cursorThemes;
}

//Window Decorations.
QList<Decoration> getWindowDecorations()
{
    /*There are two ways that window decoration is written that i know of
    using the
    "library = org.kde.nameofthelibrary"  then the "theme name" stored in /usr/lib/qt/plugins/org.kde.kdecoration2/  or
    "library =library=org.kde.kwin.aurorae"  then "theme=__aurorae__svg__nameoftheme"
    but i am not sure how to get the actual name of the library and the theme it uses
    its kinda hacky but i did it this way , if there is a better way tell me and i would update this  */
    QList<Decoration> dt;
    QDir sysLib; //for the library

    QString pluginDir(QLibraryInfo::location(QLibraryInfo::PluginsPath));
    QDir dir(pluginDir + "/org.kde.kdecoration2/");
    //TODO delete path variable just for debugging
    QString path(dir.absolutePath());
    qDebug() << "the plugin path " << path;
    if (dir.exists()) {
        sysLib = dir;
    }
    QFileInfoList libInfoTheme = sysLib.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    QStringList libThemes;
    for (const auto &file : qAsConst(libInfoTheme)) {
        libThemes.append(file.baseName());
    }
    if (libThemes.contains("kwin5_aurorae")) {
        libThemes.removeAt(libThemes.indexOf("kwin5_aurorae"));
    }
    for (auto &theme : libThemes) {
        if (theme.endsWith("decoration", Qt::CaseInsensitive)) {
            theme.chop(10);
        }
    }
    //for aurorae styles
    QStringList auroraeStyles;
    QDir aurLocalLib(QDir::homePath() + "/.local/share/aurorae/themes");
    QDir aurSysLib("/usr/share/aurorae/themes/");
    if (aurSysLib.exists()) {
        auroraeStyles.append(aurSysLib.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    }
    if (aurLocalLib.exists()) {
        auroraeStyles.append(aurLocalLib.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    }
    auroraeStyles.removeDuplicates();

    for (const auto &th: auroraeStyles) {
        Decoration d{};
        d.name = th;
        d.library = "org.kde.kwin.aurorae";
        d.theme = "__aurorae__svg__" + th;
        dt.append(d);
    }
    for (const auto &th: libThemes) {
        Decoration d{};
        d.name = th;
        d.name.replace(0, 1, d.name[0].toUpper());
        d.library = "org.kde." + d.name.toLower();
        dt.append(d);
    }
    return dt;
}

QStringList getWindowDecorationsStyle()
{
    QList<Decoration> dt = getWindowDecorations();
    QStringList styleList;
    for (const auto &style: qAsConst(dt)) {
        styleList.append(style.name);
    }
    styleList.sort();
    return styleList;
}

namespace noUse
{
void setWallpaper(const QVariant &wallFile)
{
    QDBusMessage m = QDBusMessage::createMethodCall("org.kde.plasmashell",
                                                    "/PlasmaShell",
                                                    "org.kde.PlasmaShell",
                                                    "evaluateScript");
    QString script;
    script = "var Desktops = desktops();";
    script += "for (i=0;i<Desktops.length;i++) {";
    script += "d = Desktops[i];";

    auto no = wallFile.toInt();
    if (no == 0) {
        script += "d.wallpaperPlugin = \"org.kde.image\";";
        script += "d.currentConfigGroup = Array(\"Wallpaper\", \"org.kde.image\", \"General\");";
        script += "d.writeConfig(\"Image\", \"file://";
    }
    else {
        script += "d.wallpaperPlugin = \"org.kde.potd\";";
        script += "d.currentConfigGroup = Array(\"Wallpaper\",\"org.kde.potd\", \"General\");";
        script += "d.writeConfig(\"Provider\", \"unsplash\");";
        script += "d.writeConfig(\"Category\", \"";
    }
    script += wallFile.toString();
    script += "\");}";
    m.setArguments({script});
    QDBusConnection::sessionBus().send(m);
}

}
}
