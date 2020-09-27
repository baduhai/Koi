#ifndef SCHEDULEPROFILE_H
#define SCHEDULEPROFILE_H

//KDE
#include <KPageDialog>
//Qt
#include <QWidget>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTimeEdit>

//Koi
#include "profiles/profilemanager.h"
#include "utils.h"

enum scheduleColumns {
    FavNameColumn = 0,
    TimeColumn,
};
enum schedulePages {
    CustomPage,
    SunsetSunrisePage,
};


namespace Ui {
    class ScheduleProfile;
}

class ScheduleProfile : public QWidget {
Q_OBJECT

public:
    explicit ScheduleProfile(QWidget *parent = nullptr);

    ~ScheduleProfile() override;

public slots:

    void populateView();

    void timeChanged(QStandardItem *item);

    void saveSettings();

    void switchPages(const QString &schedType);

private slots:

    void favouritesChanged(const QString &profileName, const bool &isFav);

    void enableProfile(const QModelIndex &index);

private:
    ////--Name -----Time
    QHash<QString, QString> m_editedFavourites;

    QSettings m_settings;

    void createView();

    Ui::ScheduleProfile *ui;

    QStandardItemModel *_profileTimeModel;
    void addItems(const QString &favName, const QString &favTime, const QList<QStandardItem *> &item);

    static QString enumToQString(schedulePages type) {
        switch (type) {
            case CustomPage:
                return "custom time";
            case SunsetSunrisePage:
                return "sunset and sunrise";
            default:
                return QString();
        }
    }
};

///For the Time column spinbox to be able to be edited, and shown.
class FavTimeDelegate : public QStyledItemDelegate {
Q_OBJECT

public:
    FavTimeDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // SCHEDULEPROFILE_H
