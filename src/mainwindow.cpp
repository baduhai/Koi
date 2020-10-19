#include "mainwindow.h"
#include "ui_mainwindow.h"

Bosma::Scheduler s(2);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      _trayIcon(new QSystemTrayIcon(this)),
      _trayMenu(new QMenu(this)),
      favList(ProfileManager::instance()->allProfiles()),
      _settingDialog(new SettingDialog(this)),
      isSetup(false)
{
    //won't tile if you are using a tiling script or wm.
    setAttribute(Qt::WA_X11NetWmWindowTypeDialog, true);
    loadSystemTray();

    // TODO use an enum for this maybe ?
    if (m_settings.value("schedule").toString() == "custom time") {
        runSchedule();
    }

    ui->resMsg->hide();
    auto actionRes = new QAction("Restart", this);
    actionRes->setIcon(QIcon::fromTheme("view-refresh"));
    connect(actionRes, &QAction::triggered, this, &MainWindow::on_actionRestart_triggered);
    ui->resMsg->addAction(actionRes);

    auto *manager = ProfileManager::instance();
    connect(manager, &ProfileManager::favouritesChanged ,[this](){});
}

MainWindow::~MainWindow()
{
    this->setVisible(false);
    delete ui;
}

// Override window managing events
void MainWindow::closeEvent(QCloseEvent *event)
{ // Override close event
    event->ignore();
    toggleVisibility();
}
void MainWindow::runSchedule()
{
    QString currentName(utils::startupTimeCheck()); // get the profile to be used.
    if (!currentName.isEmpty() || !currentName.isNull()) {
        auto currentProfile = ProfileManager::instance()->getProfile(currentName);
        utils::go(currentProfile);
    }

    auto manager = ProfileManager::instance();
    //Schedule other Profiles.
    m_settings.beginGroup("Favourites");

    for (const auto profile : qAsConst(favList)) {
        if (manager->isFavourite(profile->name())) {
            auto favTime = QTime::fromString(m_settings.value(profile->name()).toString());
            if (!favTime.isNull()) {
                //TODO reimplement schedule
                schedule(profile, favTime);
            }
        }
    }
    m_settings.endGroup();
}

// SysTray related functionality
QMenu *MainWindow::createMenu() // Define context menu items for SysTray - R-click to show context menu
{
    // Build tray items when you rightclick
    auto trayMenu1 = new QMenu(this);
    utils::noUse::setKonsoleTheme("tet");
    auto actionMenuToggle = new QAction("&Settings", this);
    connect(actionMenuToggle, &QAction::triggered, this, &MainWindow::on_prefsBtn_clicked);
    //disable when settings page is shown
    connect(this, &MainWindow::showSettingsTray, actionMenuToggle, &QAction::setEnabled);
    actionMenuToggle->setIcon(QIcon::fromTheme(QStringLiteral("preferences-other-symbolic")));
    trayMenu1->addAction(actionMenuToggle);
    trayMenu1->addSeparator();

    for (const auto &fav: favList) {
        auto actionFavMenu = new QAction(fav->name(), this); // Quit app
        connect(actionFavMenu, &QAction::triggered, [this, actionFavMenu]
        { profileEnabled(actionFavMenu->text()); });
        actionFavMenu->setIcon(QIcon::fromTheme(QStringLiteral("tag-people")));
        trayMenu1->addAction(actionFavMenu);
    }

    auto actionMenuQuit = new QAction(QStringLiteral("&Quit"), this); // Quit app
    connect(actionMenuQuit, &QAction::triggered, this, &QCoreApplication::quit);
    actionMenuQuit->setIcon(QIcon::fromTheme(QStringLiteral("exit")));
    trayMenu1->addSeparator();
    trayMenu1->addAction(actionMenuQuit);
    return trayMenu1;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) // Define actions for SysTray L&M-click
{
    switch (reason) {
        case QSystemTrayIcon::Trigger: // Left-click to toggle window visibility
            toggleVisibility();
            break;
        case QSystemTrayIcon::MiddleClick: // Middle-click to toggle between light and dark

            utils::notify("Hello!", "You middle-clicked me", 0); // Must implement toggle
            break;

            // Must understand tray better - Why can't right click be part of switch statement?

        default: // Need to understand switch statements better - Why is this required?
            break;
    }
}

// Independent functions
void MainWindow::toggleVisibility()
{
    if (this->isVisible() == 0) {
        this->setVisible(true);
        this->activateWindow();
    }
    else {
        this->setVisible(false);
    }

}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) // Define actions for SysTray L&M-click
{
    switch (reason) {
        case QSystemTrayIcon::Trigger: // Left-click to toggle window visibility
            if (!isSetup) {
                setupUi();
                isSetup = true;
            }
            toggleVisibility();
            break;
        case QSystemTrayIcon::MiddleClick: // Middle-click to toggle between light and dark
            utils::notify("Hello!", "You middle-clicked me", 0); // Must implement toggle
            break;
            // Must understand tray better - Why can't right click be part of switch statement?
        default: // Need to understand switch statements better - Why is this required?
            break;
    }
}

// Functionality of buttons - Related to program navigation, interaction and saving settings
void MainWindow::on_prefsBtn_clicked() // Preferences button - Sets all preferences as found in koi.conf// file
{
    auto *dialog = new SettingDialog(this);
    connect(dialog, &QDialog::finished, [this](){emit showSettingsTray(true);});
    dialog->open();
    emit showSettingsTray(false);
}
//TODO replace this later
void MainWindow::on_lightBtn_clicked()
{
    QString currentName("light"); // get the profile to be used.
    auto currentProfile = ProfileManager::instance()->getProfile(currentName);
    utils::go(currentProfile);
}

void MainWindow::profileEnabled(const QString &name)
{
    auto currentProfile = ProfileManager::instance()->getProfile(name);
    Q_ASSERT(currentProfile);
    utils::go(currentProfile);
}

void MainWindow::on_darkBtn_clicked()
{
    QString currentName("dark"); // get the profile to be used.
    auto currentProfile = ProfileManager::instance()->getProfile(currentName);
    utils::go(currentProfile);
}


//}

// Menubar actions
void MainWindow::on_actionQuit_triggered() // Quit app
{
    QApplication::quit();
}

void MainWindow::on_actionPrefs_triggered() // Set preferences
{
    on_prefsBtn_clicked(); // Triggers "Preferences" button
}

void MainWindow::on_actionAbout_triggered() // Open about dialog
{
    auto *about = new About(this);
    about->open();
}
void MainWindow::on_actionHide_triggered() // Hide to tray
{
    this->setVisible(false);
}
void MainWindow::on_actionRestart_triggered()
{
    QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
    exit(12);
}

