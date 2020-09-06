//
// Created by da-viper on 03/09/2020.
//
/* it inherits the kpagewidge this is where all the profiles are edited and added
 * it has three widget item
 * -Styles, -Others, -External.
 */
#include "profilesettingsdialog.h"

ProfileSettingsDialog::ProfileSettingsDialog(QWidget *parent, QSettings *pSettings)
	:
	QWidget(parent),
	ui(new Ui::ProfileSettingsDialog),
	settings(pSettings),
	_profileListModel(new QStandardItemModel(this))
{
	ui->setupUi(this);
	createTable();

	populateTable();
}

ProfileSettingsDialog::~ProfileSettingsDialog()
{
	delete ui;
}

void ProfileSettingsDialog::createTable()
{
	Q_ASSERT(!ui->profilesList->model());

	ui->profilesList->setModel(_profileListModel);
	_profileListModel->clear();

	//Add Headers thi order  Favorites  ProfileName
	QStringList headerNames ({ QString() , "Name"});
	_profileListModel->setHorizontalHeaderLabels(headerNames);

	auto *favoriteColumnHeaderItem = new QStandardItem();
	favoriteColumnHeaderItem->setIcon(QIcon::fromTheme(QStringLiteral("visibility")));
	favoriteColumnHeaderItem->setToolTip("Select Favorites ");
	_profileListModel->setHorizontalHeaderItem(FavoriteStatusColumn, favoriteColumnHeaderItem);

	ui->profilesList->horizontalHeader()->setSectionResizeMode(FavoriteStatusColumn , QHeaderView::ResizeToContents);
	ui->profilesList->horizontalHeader()->setSectionResizeMode(ProfileNameColumn , QHeaderView::Stretch);

	ui->profilesList->verticalHeader()->setSectionsMovable(false);
	ui->profilesList->verticalHeader()->setHidden(true);
	ui->profilesList->horizontalHeader()->setFirstSectionMovable(false);
}
void ProfileSettingsDialog::populateTable()
{
	QList<const Profile*> profileList = ProfileManager::instance()->allProfiles();

	for (const auto p : profileList) {
		addItems(p);
	}

}

// Add the Profile to the Table view
void ProfileSettingsDialog::addItems(const Profile *p)
{
	//profile must have a name.
	if (p->name().isEmpty()) {
		return;
	}

	// each _sessionModel row has three items.
	const auto items = QList<QStandardItem *>{
		new QStandardItem(), // Favorites.
		new QStandardItem(), // Profile Name.
	};

	updateItemsForProfile(p, items);
	_profileListModel->appendRow(items);
}

// Put the profile attributes(favourites , profile Name) in a standard item to show it on the table view
void ProfileSettingsDialog::updateItemsForProfile(const Profile *p, const QList<QStandardItem *> &items) const
{
	// "Enabled" checkbox
	const auto isEnabled = true;
	items[FavoriteStatusColumn]->setCheckState(isEnabled ? Qt::Checked : Qt::Unchecked);
	items[FavoriteStatusColumn]->setCheckable(true);

	// Profile Name
	items[ProfileNameColumn]->setText(p->name());
	// only allow renaming the profile from the edit profile dialog
	// so as to use ProfileManager::checkProfileName()
	items[ProfileNameColumn]->setEditable(false);
}


