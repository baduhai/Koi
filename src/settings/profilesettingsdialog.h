//
// Created by da-viper on 03/09/2020.
//

#ifndef PROFILESETTINGSDIALOG_H
#define PROFILESETTINGSDIALOG_H

#include <QSettings>
#include <QWidget>
#include <QStandardItemModel>
#include <QtWidgets/QStyledItemDelegate>
#include <QPointer>

//koi
#include "ui_profilesettingsdialog.h"
#include "profiles/profilemanager.h"
#include "profiles/editprofiledialog.h"


class QItemSelection;
class QStandardItem;
class QStandardItemModel;
class EditProfileDialog;

namespace Ui
{class ProfileSettingsDialog;
}
class ProfileSettingsDialog: public QWidget
{
Q_OBJECT
public:
	ProfileSettingsDialog(QWidget *parent, QSettings *pSettings);
	~ProfileSettingsDialog() override;


private slots:
    void addNewProfile();
    void rowSelected(const QModelIndex &index );
    void editCurrentProfile();


private:
	Ui::ProfileSettingsDialog *ui;
	QSettings *settings;

	EditProfileDialog *_ProfileDGUi;
	QStandardItemModel *_profileListModel;

	enum Column
	{
		FavouriteStatusColumn = 0,
		ProfileNameColumn = 1,
	};

//	enum Role
//	{
//		ProfilePtrRole = Qt::UserRole + 1,
//		ShortcutRole,
//	};

	void createTable();
	void populateTable();
	void addItems(const Profile *p);
	void updateItemsForProfile(const Profile *p, const QList<QStandardItem *> &items) const;
};


#endif //PROFILESETTINGSDIALOG_H
