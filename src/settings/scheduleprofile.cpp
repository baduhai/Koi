#include "scheduleprofile.h"
#include "ui_scheduleprofile.h"

ScheduleProfile::ScheduleProfile(QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::ScheduleProfile),
	_profileTimeModel(new QStandardItemModel(this))
{
	ui->setupUi(this);
	createview();
}

ScheduleProfile::~ScheduleProfile()
{
	delete ui;
}
void ScheduleProfile::createview()
{
	ui->profleTimeView->setModel(_profileTimeModel);

	//Add Headers this order:  Favorites  ProfileName
	QStringList headerNames({QString(), "Name"});
	_profileTimeModel->setHorizontalHeaderLabels({"Profile Name ", "Start Time"});

	ui->profleTimeView->horizontalHeader()->setSectionResizeMode(ProfileColumn, QHeaderView::Stretch);
	ui->profleTimeView->horizontalHeader()->setSectionResizeMode(TimeColumn, QHeaderView::ResizeToContents);
}
void ScheduleProfile::populateView()
{
	auto profileList = ProfileManager::instance()->listFavourites();
	for (const auto p : profileList) {
		QHashIterator<QString, QString> i(profileList);
		while (i.hasNext()) {
			i.next();
			if(i.key().isEmpty()){
				return;
			}

			const auto items = QList<QStandardItem *>{
				new QStandardItem(), // ProfileName.
				new QStandardItem(), // Start Time.
			};

		}
		//updateFavouriteItems(p, items);
		//_profileTimeModel->appendRow(items);
	}

}
void ScheduleProfile::addItems()
{

}

//// Add the Profile to the Table view
//void ProfileSettingsDialog::addItems(const Profile *p)
//{
//	//profile must have a name.
//	if (p->name().isEmpty()) {
//		return;
//	}
//
//
//	// each _sessionModel row has three items.
//	const auto items = QList<QStandardItem *>{
//		new QStandardItem(), // Favorites.
//		new QStandardItem(), // Profile Name.
//	};
//
//	updateItemsForProfile(p, items);
//	_profileListModel->appendRow(items);
//
//}
//
//// Put the profile attributes(favourites , profile Name) in a standard item to show it on the table view
//void ProfileSettingsDialog::updateItemsForProfile(const Profile *p, const QList<QStandardItem *> &items) const
//{
//	// "Enabled" checkbox
//	const auto isEnabled(ProfileManager::instance()->isFavourite(p->name()));
//	items[FavouriteStatusColumn]->setCheckState(isEnabled ? Qt::Checked : Qt::Unchecked);
//	items[FavouriteStatusColumn]->setSelectable(false);
//	if ((p->name() == "light") || p->name() == "dark") {
//		items[FavouriteStatusColumn]->setCheckState(Qt::Checked);
//		items[FavouriteStatusColumn]->setCheckable(false);
//		items[FavouriteStatusColumn]->setEnabled(false);
//	}
//	else {
//		items[FavouriteStatusColumn]->setCheckable(true);
//	}
//
//	// Profile Name
//	items[ProfileNameColumn]->setText(p->name());
//	// only allow renaming the profile from the edit profile dialog
//	// so as to use ProfileManager::checkProfileName()
//	items[ProfileNameColumn]->setEditable(false);
//}

