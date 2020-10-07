#include "scheduleprofile.h"
#include "ui_scheduleprofile.h"

ScheduleProfile::ScheduleProfile(QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::ScheduleProfile),
	_profileTimeModel(new QStandardItemModel(this))
{
	ui->setupUi(this);
	createView();
	populateView();
	
	ProfileManager *manager = ProfileManager::instance();

	connect(_profileTimeModel, &QStandardItemModel::itemChanged, this, &ScheduleProfile::timeChanged);
	connect(ui->profleTimeView, &QTableView::doubleClicked, this, &ScheduleProfile::enableProfile);
	connect(manager, &ProfileManager::favouritesChanged, this, &ScheduleProfile::favouritesChanged);
    connect(ui->schedTypeBox, &QComboBox::currentTextChanged, this, &ScheduleProfile::switchPages );

    //TODO: renable this after implementing the sunset and sunrise
    ui->schedTypeBox->setEnabled(false);

}

ScheduleProfile::~ScheduleProfile()
{
	delete ui;
}
void ScheduleProfile::createView()
{
	ui->profleTimeView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->profleTimeView->setModel(_profileTimeModel);

    //the type of schedule
    ui->schedTypeBox->addItems({enumToQString(CustomPage), enumToQString(SunsetSunrisePage)});

	ui->profleTimeView->setItemDelegateForColumn(TimeColumn, new FavTimeDelegate(this));

	//Add Headers this order:  Favorites  ProfileName
	QStringList headerNames({QString(), "Name"});
	_profileTimeModel->setHorizontalHeaderLabels({"Profile Name ", "Start Time"});
    _profileTimeModel->horizontalHeaderItem(1)->setToolTip("The start time of the profile.");

	ui->profleTimeView->horizontalHeader()->setSectionResizeMode(FavNameColumn, QHeaderView::Stretch);
	ui->profleTimeView->horizontalHeader()->setSectionResizeMode(TimeColumn, QHeaderView::ResizeToContents);
	ui->profleTimeView->verticalHeader()->setHidden(true);

}
void ScheduleProfile::populateView()
{

	auto profileList = ProfileManager::instance()->getFavouritesList();

	qDebug() << "this is the profile list " << profileList;
	QHashIterator<QString, QString> i(profileList);
	while (i.hasNext()) {
		i.next();
		if (i.key().isEmpty()) {
			return;
		}

		const auto items = QList<QStandardItem *>{
			new QStandardItem(), // ProfileName.
			new QStandardItem(), // Start Time.
		};

		addItems(i.key(), i.value(), items);
		auto isMatch = _profileTimeModel->findItems(i.key(), Qt::MatchExactly, FavNameColumn);

		if (isMatch.isEmpty()) {
			_profileTimeModel->appendRow(items);
		}
	}
}
void ScheduleProfile::addItems(const QString &favName, const QString &favTime, const QList<QStandardItem *> &items)
{
	items[FavNameColumn]->setText(favName);
	items[FavNameColumn]->setEditable(false);

	items[TimeColumn]->setText(favTime);
}

//not sure if i should save it automatically or not.
void ScheduleProfile::timeChanged(QStandardItem *item)
{
	qDebug() << "the item " << item;
	if (item->column() == TimeColumn) {
		QString favTime = item->model()->data(item->index(), Qt::DisplayRole).toString();
		QString profileName = item->model()->item(item->row(), FavNameColumn)->data(Qt::DisplayRole).toString();
		//don't know a better way to do this .
		if (m_editedFavourites.contains(profileName)) {
			m_editedFavourites.remove(profileName);
			m_editedFavourites.insert(profileName, favTime);
		}
		else {
			m_editedFavourites.insert(profileName, favTime);
		}
	}
}
void ScheduleProfile::saveSettings()
{
	m_settings.beginGroup("Favourites");
	QHashIterator<QString, QString> favIt(m_editedFavourites);
	while (favIt.hasNext()) {
		favIt.next();
		m_settings.setValue(favIt.key(), favIt.value());
	}
	m_settings.endGroup();
	m_settings.setValue("schedule", ui->schedTypeBox->currentText());
	m_settings.sync();
}

void ScheduleProfile::switchPages(const QString &schedType)
{
    if(schedType == enumToQString(CustomPage)) {
        ui->stackedWidget->setCurrentIndex(CustomPage);
    }else if( schedType == enumToQString(SunsetSunrisePage)){
        ui->stackedWidget->setCurrentIndex(SunsetSunrisePage);
    }

}
void ScheduleProfile::enableProfile(const QModelIndex &index)
{
	auto s = index.data(Qt::DisplayRole);
	if (index.column() == FavNameColumn) {
		auto profile = ProfileManager::instance()->getProfile(index.data(Qt::DisplayRole).toString());
		Utils utils(profile);
		utils.go();
	}
}

void ScheduleProfile::favouritesChanged(const QString &profileName, const bool &isFav)
{
	if (!isFav) {
		auto isMatch = _profileTimeModel->findItems(profileName, Qt::MatchExactly, FavNameColumn);
		for (const auto &item : isMatch) {
			_profileTimeModel->removeRow(item->row());
		}
	}
	if (isFav) {
		populateView();
	}
}
/*
 * The Time Spin Box area
 */
FavTimeDelegate::FavTimeDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{

}
QWidget *FavTimeDelegate::createEditor(QWidget *parent,
									   const QStyleOptionViewItem &/* option */,
									   const QModelIndex &/* index */) const
{
	QTimeEdit *editor = new QTimeEdit(parent);
	editor->setFrame(false);
	return editor;
}
void FavTimeDelegate::setEditorData(QWidget *editor,
									const QModelIndex &index) const
{
	QString value = (index.model()->data(index, Qt::EditRole).toString());
	QTime indexTime = QTime::fromString(value);

	//convert from QWidget to timeEdit widget
	auto *spinBox = dynamic_cast<QTimeEdit *>(editor);
	spinBox->setTime(indexTime);
	spinBox->setDisplayFormat("HH:mm");
	qDebug() << "the box " << spinBox->time();
}
void FavTimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
								   const QModelIndex &index) const
{
	auto *spinBox = dynamic_cast<QTimeEdit *>(editor);
	spinBox->interpretText();
	spinBox->setDisplayFormat("HH:mm");
	auto value = spinBox->time().toString("HH:mm");
	model->setData(index, value, Qt::EditRole);
}
void FavTimeDelegate::updateEditorGeometry(QWidget *editor,
										   const QStyleOptionViewItem &option,
										   const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}
