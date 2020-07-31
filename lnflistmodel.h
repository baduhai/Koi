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

#ifndef LNFLISTMODEL_H
#define LNFLISTMODEL_H

#include <QAbstractItemView>

namespace Plasma
{
}

//Theme selector code by Andre Duffeck (modified to add package description)
class ThemeInfo
{
public:
    QString name;
    QString package;
    QString description;
    QString author;
    QString version;
    QString themeRoot;
};

class LnfListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum { PackageNameRole = Qt::UserRole,
           PackageDescriptionRole = Qt::UserRole + 1,
           PackageAuthorRole = Qt::UserRole + 2,
           PackageVersionRole = Qt::UserRole + 3
         };

    explicit LnfListModel(QObject *parent = nullptr);
    ~LnfListModel() override;

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex indexOf(const QString &path) const;
    void reload();
    void clearThemeList();
    int count() const
    {
        return rowCount();
    }

    Q_INVOKABLE QVariantMap get(int index) const;

Q_SIGNALS:
    void countChanged();

private:
    QHash<int, QByteArray> m_roleNames;

    QList<ThemeInfo> m_themes;
};


#endif
