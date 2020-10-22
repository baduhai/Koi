#include "mainwindow.h"
#include "ui_mainwindow.h"

Bosma::Scheduler s(2);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      isSetup(false),
      ui(new Ui::MainWindow),
      _trayIcon(new QSystemTrayIcon(this)),
      _trayMenu(new QMenu(this)),
      _settingDialog(new SettingDialog(this)),
      favList(ProfileManager::instance()->allProfiles())
{
    //won't tile if you are using a tiling script or wm.
    setAttribute(Qt::WA_X11NetWmWindowTypeDialog, true);
    loadSystemTray();

    runSchedule();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _settingDialog;
    delete _trayMenu;
    delete _trayIcon;
}

void MainWindow::setupUi()
{
    ui->setupUi(this);

    //change theme when you click the light or dark button
    connect(ui->lightBtn, &QPushButton::clicked, [this]()
    { profileEnabled(ui->lightBtn->text().toLower()); });
    connect(ui->darkBtn, &QPushButton::clicked, [this]()
    { profileEnabled(ui->darkBtn->text().toLower()); });
    connect(ui->prefsBtn, &QPushButton::clicked, this, &MainWindow::showSettingsDialog);
    //Menubar Actions
    connect(ui->actionPrefs, &QAction::triggered, this, &MainWindow::showSettingsDialog);//Preferences
    connect(ui->actionHide, &QAction::triggered, this, &QMainWindow::hide);//Hide
    connect(ui->actionQuit, &QAction::triggered, this, &QApplication::quit);//Quit

    ui->resMsg->hide();
    auto actionRes = new QAction("Restart", this);
    actionRes->setIcon(QIcon::fromTheme("view-refresh"));
    connect(actionRes, &QAction::triggered, this, &MainWindow::on_actionRestart_triggered);
    ui->resMsg->addAction(actionRes);

}
// Override window managing events
void MainWindow::closeEvent(QCloseEvent *event)
{ // Override close event
    event->ignore();
    toggleVisibility();
}

void MainWindow::runSchedule()
{
    // TODO use an enum for this maybe ?
    if (m_settings.value("schedule").toString() == "custom time") {
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
}

// SysTray related functionality
void MainWindow::createMenu() // Define context menu items for SysTray - R-click to show context menu
{
    // Build tray items when you rightclick
    auto actionMenuToggle = new QAction("&Settings", this);
    connect(actionMenuToggle, &QAction::triggered, this, &MainWindow::showSettingsDialog);
    //disable when settings page is shown
    connect(this, &MainWindow::showSettingsTray, actionMenuToggle, &QAction::setEnabled);
    actionMenuToggle->setIcon(QIcon::fromTheme(QStringLiteral("preferences-other-symbolic")));
    _trayMenu->addAction(actionMenuToggle);
    _trayMenu->addSeparator();

    for (const auto &fav: qAsConst(favList)) {
        auto actionFavMenu = new QAction(fav->name(), this); // Quit app
        connect(actionFavMenu, &QAction::triggered, [this, actionFavMenu]
        { profileEnabled(actionFavMenu->text()); });
        actionFavMenu->setIcon(QIcon::fromTheme(QStringLiteral("tag-people")));
        _trayMenu->addAction(actionFavMenu);
    }

    auto actionMenuQuit = new QAction(QStringLiteral("&Quit"), this); // Quit app
    connect(actionMenuQuit, &QAction::triggered, this, &QCoreApplication::quit);
    actionMenuQuit->setIcon(QIcon::fromTheme(QStringLiteral("exit")));
    _trayMenu->addSeparator();
    _trayMenu->addAction(actionMenuQuit);
}

void MainWindow::loadSystemTray()
{
    _trayIcon->setIcon(QPixmap(":/resources/icons/koi_tray.svg")); // Set tray icon - Not sure why svg doesn't work
    _trayIcon->setVisible(true);
    createMenu();
    _trayIcon->setContextMenu(_trayMenu);   // Set tray context menu

    connect(_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated); // System tray interaction
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

void MainWindow::schedule(Profile *p, QTime time)
{
    auto favTime = time;
    int cronMin = (favTime.minute() < 0) ? 0 : favTime.minute();
    int cronHr = (favTime.hour() < 0) ? 0 : favTime.hour();

    std::string cronJ = std::to_string(cronMin) + " " + std::to_string(cronHr) + " * * *";
    s.cron(cronJ, [p]()
    {
        utils::go(p);
    });
}

// Functionality of buttons - Related to program navigation, interaction and saving settings
void MainWindow::showSettingsDialog() // Preferences button - Sets all preferences as found in koi.conf// file
{
    connect(_settingDialog, &QDialog::finished, [this]()
    { emit showSettingsTray(true); });
    _settingDialog->open();
    emit showSettingsTray(false);
}

void MainWindow::profileEnabled(const QString &name)
{
    auto currentProfile = ProfileManager::instance()->getProfile(name);
    Q_ASSERT(currentProfile);
    utils::go(currentProfile);
}

// Menubar actions
void MainWindow::on_actionAbout_triggered() // Open about dialog
{
    auto *about = new About(this);
    about->open();
}
void MainWindow::on_actionRestart_triggered()
{
    QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
    exit(12);
}
void MainWindow::showWindow()
{
    if (m_settings.value("start-hidden").toBool()) {
        hide();
    }
    else {
        setupUi();
        show();
    }

}
