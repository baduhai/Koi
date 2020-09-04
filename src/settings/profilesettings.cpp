//
// Created by da-viper on 03/09/2020.
//
/* it inherits the kpagewidge this is where all the profiles are edited and added
 * it has three widget item
 * -Styles, -Others, -External.
 */
#include "profilesettings.h"

ProfileSettings::ProfileSettings(QWidget *parent, QSettings *pSettings)
	:
	QWidget(parent),
	ui(new Ui::ProfileSettings),
	settings(pSettings),
	_sessionModel(new QStandardItemModel(this))
{
	ui->setupUi(this);
	ui->profilesList->setItemDelegateForColumn(ShortcutColumn, new ShortcutItemDelegate(this));
	populateTable();
}

ProfileSettings::~ProfileSettings() noexcept
{
	delete ui;
}
void ProfileSettings::populateTable()
{
    Q_ASSERT(!ui->profilesList->model());

    ui->profilesList->setModel(_sessionModel);

    _sessionModel->clear();
    // setup session table
    _sessionModel->setHorizontalHeaderLabels({
        QString(), // set using header item below
        ( "Name"),
        ("Shortcut"),
        QString(),
    });
    auto *favoriteColumnHeaderItem = new QStandardItem();
    favoriteColumnHeaderItem->setIcon(QIcon::fromTheme(QStringLiteral("visibility")));
    favoriteColumnHeaderItem->setToolTip("Show profile in menu");
    _sessionModel->setHorizontalHeaderItem(FavoriteStatusColumn, favoriteColumnHeaderItem);

    // Calculate favorite column width. resizeColumnToContents()
    // is not used because it takes distance between checkbox and
    // text into account, but there is no text and it looks weird.
    const int headerMargin = style()->pixelMetric(QStyle::PM_HeaderMargin, nullptr,
                                            ui->profilesList->header());
    const int iconWidth = style()->pixelMetric(QStyle::PM_SmallIconSize, nullptr,
                                               ui->profilesList->header());
    const int favoriteHeaderWidth = headerMargin * 2 + iconWidth;
    QStyleOptionViewItem opt;
    opt.features = QStyleOptionViewItem::HasCheckIndicator | QStyleOptionViewItem::HasDecoration;
    const QRect checkBoxRect = style()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator,
                                                       &opt, ui->profilesList);
    // When right edge is at x < 0 it is assumed the checkbox is
    // placed on the right item's side and the margin between right
    // checkbox edge and right item edge should be used.
    const int checkBoxMargin = checkBoxRect.right() >= 0 ? checkBoxRect.x()
                                                         : 0 - checkBoxRect.right();
    const int favoriteItemWidth = checkBoxMargin * 2 + checkBoxRect.width();
    auto *listHeader = ui->profilesList->header();

    ui->profilesList->setColumnWidth(FavoriteStatusColumn,
                                 qMax(favoriteHeaderWidth, favoriteItemWidth));
    ui->profilesList->resizeColumnToContents(ProfileNameColumn);
    listHeader->setSectionResizeMode(FavoriteStatusColumn, QHeaderView::ResizeMode::Fixed);
    listHeader->setSectionResizeMode(ProfileNameColumn, QHeaderView::ResizeMode::Stretch);
    listHeader->setSectionResizeMode(ShortcutColumn, QHeaderView::ResizeMode::ResizeToContents);
    listHeader->setStretchLastSection(false);
    listHeader->setSectionsMovable(false);

    QList<QStringList> theList ({{"1","Breeze", "yes"}, {"false","Material", "no"}, {"4", "Qogir","that"}});
    for(const auto &sList : theList){
    	addItems(sList);
    }

}
void ProfileSettings::addItems(const QStringList &list)
{
    if (list.isEmpty()) {
        return;
    }

    // each _sessionModel row has three items.
    const auto items = QList<QStandardItem*> {
        new QStandardItem(),
        new QStandardItem(),
        new QStandardItem(),
      //  new QStandardItem(),
    };

    updateItemsForProfile(list, items);
    _sessionModel->appendRow(items);
}

void ProfileSettings::updateItemsForProfile(const QStringList &list, const QList<QStandardItem*>& items) const
{
    // "Enabled" checkbox
    const auto isEnabled = true;
    items[FavoriteStatusColumn]->setCheckState(isEnabled ? Qt::Checked : Qt::Unchecked);
    items[FavoriteStatusColumn]->setCheckable(true);

    // Profile Name
    items[ProfileNameColumn]->setText(list[1]);
    // only allow renaming the profile from the edit profile dialog
    // so as to use ProfileManager::checkProfileName()
    items[ProfileNameColumn]->setEditable(false);

}

ShortcutItemDelegate::ShortcutItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent),
    _modifiedEditors(QSet<QWidget *>()),
    _itemsBeingEdited(QSet<QModelIndex>())
{
}

