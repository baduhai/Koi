#include "scheduleprofile.h"
#include "ui_scheduleprofile.h"

ScheduleProfile::ScheduleProfile(QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::ScheduleProfile),
	_profileTimeModel(new QStandardItemModel (this))
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
