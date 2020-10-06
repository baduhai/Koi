//
// Created by da-viper on 03/09/2020.
//
/* it inherits the kpagewidge this is where all the profiles are edited and added
 * it has three widget item
 * -Styles, -Others, -External.
 */
#include "profilesettingsdialog.h"

ProfileSettingsDialog::ProfileSettingsDialog(QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::ProfileSettingsDialog),
	_ProfileDGUi(nullptr),
	_profileListModel(new QStandardItemModel(this))
{
	ui->setupUi(this);
	connect(ui->newProfileBtn, &QPushButton::clicked, this, &ProfileSettingsDialog::addNewProfile);
	createTable();
	ProfileManager *manager = ProfileManager::instance();
	populateTable();

	connect(_profileListModel, &QStandardItemModel::itemChanged, this, &ProfileSettingsDialog::itemDataChanged);
	connect(manager, &ProfileManager::favouritesChanged, manager, &ProfileManager::saveFavourites);

	//and disable the edit and delete button.
	// may use this for double clicked too
	connect(ui->profilesList->selectionModel(),
			&QItemSelectionModel::selectionChanged,
			this,
			&ProfileSettingsDialog::tableSelectionChanged);
	//TODO when add profile update the table view.
	//TODO when you delete profile update the table view.
	connect(ui->profilesList, &QAbstractItemView::doubleClicked, this, &ProfileSettingsDialog::editCurrentProfile);
	connect(ui->editProfileBtn, &QPushButton::clicked, this, &ProfileSettingsDialog::editCurrentProfile);
	connect(ui->deleteProfileBtn, &QPushButton::clicked, this, &ProfileSettingsDialog::deleteCurrentProfile);
}

ProfileSettingsDialog::~ProfileSettingsDialog()
{
	delete ui;
}

void ProfileSettingsDialog::addNewProfile()
{
	Profile *newProfile = new Profile();
	EditProfileDialog *dialog = new EditProfileDialog(this);
	connect(dialog, &EditProfileDialog::addNewProfile, this, &ProfileSettingsDialog::addItems);
	dialog->setProfile(newProfile);
	dialog->open();
}

void ProfileSettingsDialog::editCurrentProfile()
{
	Q_ASSERT(!currentIndex.isEmpty());

	qDebug() << "the profile selected before editing:\n " << currentIndex;
	auto *dialog = new EditProfileDialog(this);
	dialog->setProfile(ProfileManager::instance()->_activeProfile);
	qDebug() << "the glob path " << ProfileManager::instance()->_activeProfile->getGlobDir();
	qDebug() << "the config path " << ProfileManager::instance()->_activeProfile->configPath();
	dialog->open();
}

void ProfileSettingsDialog::deleteCurrentProfile()
{
	auto delManager = ProfileManager::instance();
	auto name = delManager->_activeProfile->name();

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this,
								  "Delete Profile",
								  "Are you sure you want to delete " + name + " profile ? ",
								  QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		//verify the right directory
		//TODO make sure it is the right directory
		auto isDelFav = delManager->isFavourite(name);
		delManager->deleteProfile();

		auto isMatch = _profileListModel->findItems(name, Qt::MatchExactly, ProfileNameColumn);
		for (const auto &item : isMatch) {
			_profileListModel->removeRow(item->row());
		}
		delManager->favouritesChanged(name, isDelFav);
	}
}

void ProfileSettingsDialog::createTable()
{
	Q_ASSERT(!ui->profilesList->model());
	_profileListModel->clear();

	ui->profilesList->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->profilesList->setModel(_profileListModel);

	//Add Headers this order:  Favorites  ProfileName
	QStringList headerNames({QString(), "Name"});
	_profileListModel->setHorizontalHeaderLabels(headerNames);

	auto *favoriteColumnHeaderItem = new QStandardItem();
	favoriteColumnHeaderItem->setIcon(QIcon::fromTheme(QStringLiteral("visibility")));
	favoriteColumnHeaderItem->setToolTip("Select Favorites ");
	_profileListModel->setHorizontalHeaderItem(FavouriteStatusColumn, favoriteColumnHeaderItem);

	ui->profilesList->horizontalHeader()->setSectionResizeMode(FavouriteStatusColumn, QHeaderView::ResizeToContents);
	ui->profilesList->horizontalHeader()->setSectionResizeMode(ProfileNameColumn, QHeaderView::Stretch);

	ui->profilesList->verticalHeader()->setSectionsMovable(false);
	ui->profilesList->verticalHeader()->setHidden(true);
	ui->profilesList->horizontalHeader()->setSectionsMovable(false);
}
void ProfileSettingsDialog::populateTable()
{
	QList<Profile *> profileList = ProfileManager::instance()->allProfiles();
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
	auto isMatch = _profileListModel->findItems(p->name(), Qt::MatchExactly, ProfileNameColumn);

	if (isMatch.isEmpty()) {
		_profileListModel->appendRow(items);
	}

}

// Put the profile attributes(favourites , profile Name) in a standard item to show it on the table view
void ProfileSettingsDialog::updateItemsForProfile(const Profile *p, const QList<QStandardItem *> &items) const
{
	// "Enabled" checkbox
	const auto isEnabled(ProfileManager::instance()->isFavourite(p->name()));
	items[FavouriteStatusColumn]->setCheckState(isEnabled ? Qt::Checked : Qt::Unchecked);
	items[FavouriteStatusColumn]->setSelectable(false);
	if ((p->name() == "light") || p->name() == "dark") {
		items[FavouriteStatusColumn]->setCheckState(Qt::Checked);
		items[FavouriteStatusColumn]->setCheckable(false);
		items[FavouriteStatusColumn]->setEnabled(false);
	}
	else {
		items[FavouriteStatusColumn]->setCheckable(true);
	}

	// Profile Name
	items[ProfileNameColumn]->setText(p->name());
	// only allow renaming the profile from the edit profile dialog
	// so as to use ProfileManager::checkProfileName()
	items[ProfileNameColumn]->setEditable(false);
}
void ProfileSettingsDialog::tableSelectionChanged()
{
	currentIndex = ui->profilesList->selectionModel()->currentIndex().data().toString();
	auto manager = ProfileManager::instance();
	manager->_activeProfile = manager->getProfile(currentIndex);
	bool isDefault = (currentIndex == ("dark") || (currentIndex == ("light")));
	bool isDeletable = !isDefault && !currentIndex.isEmpty();
	auto noOfRows = _profileListModel->rowCount();

	ui->newProfileBtn->setEnabled(noOfRows < 5);
	ui->editProfileBtn->setEnabled(!currentIndex.isEmpty());
	ui->deleteProfileBtn->setEnabled(isDeletable);
}

void ProfileSettingsDialog::itemDataChanged(QStandardItem *item)
{
	qDebug() << "this is the item " << item;
	if (item->column() == FavouriteStatusColumn) {
		QString profileName = item->model()->item(item->row(), ProfileNameColumn)->data(Qt::DisplayRole).toString();
		bool isFav = item->checkState() == Qt::Checked;
		qDebug() << "isFav " << isFav;
		ProfileManager::instance()->setFavourite(profileName, isFav);
	}

}

