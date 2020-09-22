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
	populateView();
}

ScheduleProfile::~ScheduleProfile()
{
	delete ui;
}
void ScheduleProfile::createview()
{
	ui->profleTimeView->setModel(_profileTimeModel);
	ui->profleTimeView->setItemDelegateForColumn(TimeColumn, new FavTimeDelegate(this));

	//Add Headers this order:  Favorites  ProfileName
	QStringList headerNames({QString(), "Name"});
	_profileTimeModel->setHorizontalHeaderLabels({"Profile Name ", "Start Time"});

	ui->profleTimeView->horizontalHeader()->setSectionResizeMode(favNameColumn, QHeaderView::Stretch);
	ui->profleTimeView->horizontalHeader()->setSectionResizeMode(TimeColumn, QHeaderView::ResizeToContents);
	ui->profleTimeView->verticalHeader()->setHidden(true);
}
void ScheduleProfile::populateView()
{
	auto profileList = ProfileManager::instance()->listFavourites();
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
		_profileTimeModel->appendRow(items);
	}
}
void ScheduleProfile::addItems(const QString &favName, const QString &favTime, const QList<QStandardItem *> &items)
{
	items[favNameColumn]->setText(favName);
	items[favNameColumn]->setEditable(false);

	items[TimeColumn]->setText(favTime);
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
	QTime indexTime;
	QString value = (index.model()->data(index, Qt::EditRole).toString());
	indexTime = QTime::fromString(value), "HH:mm";
	qDebug() << "the int " << value;
	qDebug() << "the time " << indexTime;
	//convert from QWidget to timeEdit widget
	auto *spinBox = dynamic_cast<QTimeEdit *>(editor);
	spinBox->setTime(indexTime);
}
void FavTimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
								   const QModelIndex &index) const
{
	auto *spinBox = dynamic_cast<QTimeEdit *>(editor);
	spinBox->interpretText();
	auto value = spinBox->time();

	model->setData(index, value, Qt::EditRole);
}
void FavTimeDelegate::updateEditorGeometry(QWidget *editor,
										   const QStyleOptionViewItem &option,
										   const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}