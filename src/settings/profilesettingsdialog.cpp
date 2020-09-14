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
    connect(ui->newProfileBtn , &QPushButton::clicked ,this , &ProfileSettingsDialog::addNewProfile);
	createTable();

	populateTable();
	//TODO when you don't have any profile selected, point to nothing
	//and disable the edit and delete button.
   	// may use this for double clicked too

   //TODO when add profile update the table view.
   //TODO when you delete profile update the table view.
    connect(ui->profilesList, &QTableView::clicked , this , &ProfileSettingsDialog::rowSelected);
    connect(ui->editProfileBtn, &QPushButton::clicked, this , &ProfileSettingsDialog::editCurrentProfile);
    connect(ui->deleteProfileBtn, &QPushButton::clicked, this, &ProfileSettingsDialog::deleteCurrentProfile);
//    connect(ui->deleteProfileBtn, &QPushButton::clicked, ProfileManager::instance, &ProfileManager::_activeProfile.delete);
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

void ProfileSettingsDialog::rowSelected(const QModelIndex &index)
{
    QString indexStr = ui->profilesList->model()->data(index).toString();
	qDebug() << "the index is " << index;
    qDebug()<< "The selected theme is  " << indexStr ;
    ui->editProfileBtn->setEnabled(true);
    ui->deleteProfileBtn->setEnabled(true);

    ProfileManager::instance()->_activeProfile = ProfileManager::instance()->getProfile(indexStr);
}

void ProfileSettingsDialog::editCurrentProfile()
{
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

	ui->profilesList->setModel(_profileListModel);
	_profileListModel->clear();

	//Add Headers this order:  Favorites  ProfileName
	QStringList headerNames ({ QString() , "Name"});
	_profileListModel->setHorizontalHeaderLabels(headerNames);

	auto *favoriteColumnHeaderItem = new QStandardItem();
	favoriteColumnHeaderItem->setIcon(QIcon::fromTheme(QStringLiteral("visibility")));
	favoriteColumnHeaderItem->setToolTip("Select Favorites ");
	_profileListModel->setHorizontalHeaderItem(FavouriteStatusColumn, favoriteColumnHeaderItem);

	ui->profilesList->horizontalHeader()->setSectionResizeMode(FavouriteStatusColumn , QHeaderView::ResizeToContents);
	ui->profilesList->horizontalHeader()->setSectionResizeMode(ProfileNameColumn , QHeaderView::Stretch);

	ui->profilesList->verticalHeader()->setSectionsMovable(false);
	ui->profilesList->verticalHeader()->setHidden(true);
	ui->profilesList->horizontalHeader()->setSectionsMovable(false);
}
void ProfileSettingsDialog::populateTable()
{
	QList<Profile*> profileList = ProfileManager::instance()->allProfiles();
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
	const auto isEnabled (ProfileManager::instance()->isFavourite(p->name()));
	items[FavouriteStatusColumn]->setCheckState(isEnabled ? Qt::Checked : Qt::Unchecked);
	if((p->name() == "light") || p->name() == "dark"){
		items[FavouriteStatusColumn]->setCheckState(Qt::Checked);
		items[FavouriteStatusColumn]->setCheckable(false);
		items[FavouriteStatusColumn]->setEnabled(false);
	}else{
		items[FavouriteStatusColumn]->setCheckable(true);
	}

	// Profile Name
	items[ProfileNameColumn]->setText(p->name());
	// only allow renaming the profile from the edit profile dialog
	// so as to use ProfileManager::checkProfileName()
	items[ProfileNameColumn]->setEditable(false);
}

