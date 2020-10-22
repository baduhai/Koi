//
// Created by da-viper on 16/10/2020.
//

#ifndef OTHERS_H
#define OTHERS_H

//Qt libs
#include <QStringList>
#include <QDir>
#include <QLibraryInfo>
#include <QStandardPaths>
#include <QDebug>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QSettings>

struct Decoration
{
    QString library;
    QString theme;
    QString name;
};
namespace utils
{
QStringList getIcons();
QStringList getCursorThemes();

QStringList getWindowDecorationsStyle();
QList<Decoration> getWindowDecorations();

namespace noUse
{
void setWallpaper(const QVariant &wallFile);
}
}

#endif //OTHERS_H
