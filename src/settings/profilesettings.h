//
// Created by da-viper on 03/09/2020.
//

#ifndef PROFILESETTINGS_H
#define PROFILESETTINGS_H

#include <QSettings>
#include <QWidget>
#include <QStandardItemModel>
#include <QtWidgets/QStyledItemDelegate>

#include "ui_profilesettings.h"

class QItemSelection;
class QStandardItem;
class QStandardItemModel;

//namespace Ui
//{
////class ProfileSettings;
//}
class ProfileSettings: public QWidget
{
Q_OBJECT
public:
	ProfileSettings(QWidget *parent, QSettings *pSettings);
	~ProfileSettings() override;


private:
	Ui::ProfileSettings *ui;
	QSettings *settings;

	QStandardItemModel *_profileListModel;

	enum Column
	{
		FavoriteStatusColumn = 0,
		ProfileNameColumn = 1,
	};

//	enum Role
//	{
//		ProfilePtrRole = Qt::UserRole + 1,
//		ShortcutRole,
//	};

	void createTable();
	void populateTable();
	void addItems(const QStringList &list);
	void updateItemsForProfile(const QStringList &list, const QList<QStandardItem *> &items) const;
};


#endif //PROFILESETTINGS_H
