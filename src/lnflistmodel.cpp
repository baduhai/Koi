/*
 * LnfListModel
 * Copyright (C) 2016 Marco Martin <mart@kde.org>
 * Copyright (C) 2002 Karol Szwed <gallium@kde.org>
 * Copyright (C) 2002 Daniel Molkentin <molkentin@kde.org>
 * Copyright (C) 2007 Urs Wolfer <uwolfer @ kde.org>
 * Copyright (C) 2009 by Davide Bettio <davide.bettio@kdemail.net>

 * Portions Copyright (C) 2007 Paolo Capriotti <p.capriotti@gmail.com>
 * Portions Copyright (C) 2007 Ivan Cukic <ivan.cukic+kde@gmail.com>
 * Portions Copyright (C) 2008 by Petri Damsten <damu@iki.fi>
 * Portions Copyright (C) 2000 TrollTech AS.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "lnflistmodel.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QPainter>
#include <QStandardPaths>

#include <KDesktopFile>

#include <Plasma/Theme>
#include <qstandardpaths.h>

#include <QDebug>

LnfListModel::LnfListModel( QObject *parent )
: QAbstractListModel( parent )
{
    m_roleNames.insert(Qt::DisplayRole, "displayRole");
    m_roleNames.insert(PackageNameRole, "packageNameRole");
    m_roleNames.insert(PackageDescriptionRole, "packageDescriptionRole");
    m_roleNames.insert(PackageAuthorRole, "packageAuthorRole");
    m_roleNames.insert(PackageVersionRole, "packageVersionRole");

    reload();
}

LnfListModel::~LnfListModel()
{
    clearThemeList();
}

QHash<int, QByteArray> LnfListModel::roleNames() const
{
    return m_roleNames;
}

bool LnfListModel::themeExists(const QString &name) const {
    QListIterator<ThemeInfo> it(m_themes);
    int i = -1 ;
    while (it.hasNext()){
        ++i;
        if(it.next().package == name ){
            return true;
        }
    }
    return false;
}
void LnfListModel::clearThemeList()
{
    m_themes.clear();
}

void LnfListModel::reload()
{
    beginResetModel();
    clearThemeList();

    // get all desktop themes
    QStringList themes;
    const QStringList &packs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "plasma/look-and-feel", QStandardPaths::LocateDirectory);
    foreach (const QString &ppath, packs) {
        const QDir cd(ppath);
        const QStringList &entries = cd.entryList(QDir::Dirs | QDir::Hidden);
        foreach (const QString pack, entries) {
            const QString _metadata = ppath+QLatin1Char('/')+pack+QStringLiteral("/metadata.desktop");
            if ((pack != "." && pack != "..") &&
                (QFile::exists(_metadata))) {
                themes << _metadata;
            }
        }
    }

    foreach (const QString &theme, themes) {
        int themeSepIndex = theme.lastIndexOf('/', -1);
        QString themeRoot = theme.left(themeSepIndex);
        int themeNameSepIndex = themeRoot.lastIndexOf('/', -1);
        QString packageName = themeRoot.right(themeRoot.length() - themeNameSepIndex - 1);

        KDesktopFile df(theme);

        if (df.noDisplay()) {
            continue;
        }

        QString name = df.readName();
        if (name.isEmpty()) {
            name = packageName;
        }
        const QString comment = df.readComment();
        const QString author = df.desktopGroup().readEntry("X-KDE-PluginInfo-Author",QString());
        const QString version = df.desktopGroup().readEntry("X-KDE-PluginInfo-Version",QString());

        ThemeInfo info;
        info.name = name;
        info.package = packageName;
        info.description = comment;
        info.author = author;
        info.version = version;
        info.themeRoot = themeRoot;
        m_themes << info;
    }

    endResetModel();
    emit countChanged();
}

int LnfListModel::rowCount(const QModelIndex &) const
{
    return m_themes.size();
}

QVariant LnfListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= m_themes.size() || index.row() < 0) {
        return QVariant();
    }

    switch (role) {
        case Qt::DisplayRole:
            return m_themes.value(index.row()).name;
        case PackageNameRole:
            return m_themes.value(index.row()).package;
        case PackageDescriptionRole:
            return m_themes.value(index.row()).description;
        case PackageAuthorRole:
            return m_themes.value(index.row()).author;
        case PackageVersionRole:
            return m_themes.value(index.row()).version;
        default:
            return QVariant();
    }
}
// would not use this 
QVariantMap LnfListModel::get(int row) const
{
    QVariantMap item;

    QModelIndex idx = index(row, 0);

    item["display"] = data(idx, Qt::DisplayRole);
    item["packageNameRole"] = data(idx, PackageNameRole);
    item["packageDescriptionRole"] = data(idx, PackageDescriptionRole);
    item["packageAuthorRole"] = data(idx, PackageAuthorRole);
    item["packageVersionRole"] = data(idx, PackageVersionRole);

    return item;
}

QModelIndex LnfListModel::indexOf(const QString &name) const
{
    QListIterator<ThemeInfo> it(m_themes);
    int i = -1;
    while (it.hasNext()) {
        ++i;
        if (it.next().package == name) {
            return index(i, 0);
        }
    }

    return QModelIndex();
}

QList<ThemeInfo> LnfListModel::getThemes(){
    return m_themes;
}
#include "moc_lnflistmodel.cpp"
