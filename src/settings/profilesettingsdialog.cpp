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
	_ProfileDGUi(nullptr),
	settings(pSettings),
	_profileListModel(new QStandardItemModel(this))
{
	ui->setupUi(this);
	connect(ui->newProfileBtn, &QPushButton::clicked, this, &ProfileSettingsDialog::addNewProfile);
	createTable();

	populateTable();
	//TODO when you don't have any profile selected, point to nothing
	//and disable the edit and delete button.
	// may use this for double clicked too

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
	dialog->setProfile(newProfile);
	dialog->open();
}

void ProfileSettingsDialog::editCurrentProfile()
{
	Q_ASSERT(!currentIndex.isEmpty());
	ProfileManager::_activeProfile = ProfileManager::instance()->getProfile(currentIndex);
	qDebug() << "the profile selected before editing:\n " << currentIndex;
	EditProfileDialog *dialog = new EditProfileDialog(this);
	dialog->setProfile(ProfileManager::_activeProfile);
	dialog->open();
}

void ProfileSettingsDialog::deleteCurrentProfile()
{
	//ProfileManager::instance()->_activeProfile
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
	connect(ui->profilesList->selectionModel(),
			&QItemSelectionModel::selectionChanged,
			this,
			&ProfileSettingsDialog::tableSelectionChanged);
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
	bool isDefault = (currentIndex == ("dark") || (currentIndex == ("light")));
	bool isDeletable = !isDefault && !currentIndex.isEmpty();

	ui->editProfileBtn->setEnabled(!currentIndex.isEmpty());
	ui->deleteProfileBtn->setEnabled(isDeletable);
}

