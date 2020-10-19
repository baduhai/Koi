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
{
class ProfileSettingsDialog;
}
class ProfileSettingsDialog: public QWidget
{
Q_OBJECT
public:
	explicit ProfileSettingsDialog(QWidget *parent);
	~ProfileSettingsDialog() override;

signals:
    //hides the settings Dialog if enabled
    void hideSettingsDialog(bool isHidden);
private slots:
    //Creates a EditProfile Dialog with a new profile
	void addNewProfile();
    /***Creates a EditProfileDialog with the current selected
    profile in the ProfileList tableview
    */
	void editCurrentProfile();
	void deleteCurrentProfile();
	void tableSelectionChanged();
	void itemDataChanged(QStandardItem *item);

private:
	Ui::ProfileSettingsDialog *ui;

	EditProfileDialog *_editProfileUi;
	QStandardItemModel *_profileListModel;
	QString currentIndex;

	//Table View Column types
	enum Column
	{
		FavouriteStatusColumn = 0,
		ProfileNameColumn = 1,
	};

	void createTable();
	void populateTable();
	void addItems(const Profile *p);
	void updateItemsForProfile(const Profile *p, const QList<QStandardItem *> &items) const;
};


#endif //PROFILESETTINGSDIALOG_H
