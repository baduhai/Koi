#ifndef SCHEDULEPROFILE_H
#define SCHEDULEPROFILE_H

//KDE
#include <KPageDialog>
//Qt
#include <QWidget>
#include <QStandardItemModel>

//Koi
#include "profiles/profilemanager.h"

enum scheduleColumns{
	ProfileColumn = 0,
	TimeColumn,
};

namespace Ui
{
class ScheduleProfile;
}

class ScheduleProfile: public QWidget
{
Q_OBJECT

public:
	explicit ScheduleProfile(QWidget *parent = nullptr);
	~ScheduleProfile();

public slots:
	void populateView();
private:
	void createview();
	Ui::ScheduleProfile *ui;
	QStandardItemModel *_profileTimeModel;
	void addItems();


};

#endif // SCHEDULEPROFILE_H
