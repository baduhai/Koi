//
// Created by da-viper on 03/09/2020.
//

#ifndef PROFILESETTINGS_H
#define PROFILESETTINGS_H

#include <QSettings>
#include <QWidget>
#include <QStandardItemModel>
#include <QtWidgets/QStyledItemDelegate>

#include "ui_profilesettings.h"

class QItemSelection;
class QStandardItem;
class QStandardItemModel;

//namespace Ui
//{
////class ProfileSettings;
//}
class ProfileSettings: public QWidget
{
Q_OBJECT
public:
	ProfileSettings(QWidget *parent, QSettings *pSettings);
	~ProfileSettings() override;


private:
	Ui::ProfileSettings *ui;
	QSettings *settings;

	QStandardItemModel *_sessionModel;

	enum Column {
        FavoriteStatusColumn = 0,
        ProfileNameColumn    = 1,
        ShortcutColumn       = 2,
        //ProfileColumn      = 3,
    };

	    enum Role {
        ProfilePtrRole = Qt::UserRole + 1,
        ShortcutRole,
    };
	void populateTable();
	void addItems(const QStringList &list);
	void updateItemsForProfile(const QStringList &list, const QList<QStandardItem *> &items) const;
};


class ShortcutItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ShortcutItemDelegate(QObject *parent = nullptr);
//
//    void setModelData(QWidget *editor, QAbstractItemModel *model,
//                      const QModelIndex &index) const override;
//    QWidget *createEditor(QWidget *aParent, const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const override;
//    void paint(QPainter *painter, const QStyleOptionViewItem &option,
//               const QModelIndex &index) const override;
//    QSize sizeHint(const QStyleOptionViewItem &option,
//                   const QModelIndex &index) const override;
//    void destroyEditor(QWidget *editor, const QModelIndex &index) const override;

private Q_SLOTS:
 //   void editorModified();

private:
    mutable QSet<QWidget *> _modifiedEditors;
    mutable QSet<QModelIndex> _itemsBeingEdited;
};

#endif //PROFILESETTINGS_H
