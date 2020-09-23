#include "mainwindow.h"
#include "ui_mainwindow.h"


Bosma::Scheduler s(2);

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {

	settings = new QSettings(QDir::homePath() + "/.config/koirc", QSettings::IniFormat );

    trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setIcon(QIcon(":/resources/icons/koi_tray.png")); // Set tray icon - Not sure why svg doesn't work
    this->trayIcon->setVisible(true);
    trayMenu = this->createMenu();
    this->trayIcon->setContextMenu(trayMenu);   // Set tray context menu

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated); // System tray interaction

    utils.initialiseSettings();
    ui->setupUi(this);
    if (settings->value("schedule").toBool()) {
        QString currentName (Utils::startupTimeCheck()); // get the profile to be used.
        auto currentProfile = ProfileManager::getProfile(currentName);
        Utils current(currentProfile);
        current.go();

        auto manager = ProfileManager::instance();
        //Schedule other Profiles.
        auto profileSchedList = manager->allProfiles();
        for (const auto profile : profileSchedList){
        	if(manager->isFavourite(profile->name())){
        		schedule(profile);
        	}
        }

        scheduleLight();
        scheduleDark();
    }
    ui->resMsg->hide();
    auto actionRes = new QAction("Restart", this);
    actionRes->setIcon(QIcon::fromTheme("view-refresh"));
    connect(actionRes, &QAction::triggered, this, &MainWindow::on_actionRestart_triggered);
    ui->resMsg->addAction(actionRes);
}

MainWindow::~MainWindow() {
    this->setVisible(0);
}

// Override window managing events
void MainWindow::closeEvent(QCloseEvent *event) { // Override close event
    event->ignore();
    toggleVisibility();
}

// SysTray related functionality
QMenu *MainWindow::createMenu() // Define context menu items for SysTray - R-click to show context menu
{
    // Tray action menu
    auto actionMenuQuit = new QAction("&Quit", this); // Quit app
    connect(actionMenuQuit, &QAction::triggered, this, &QCoreApplication::quit);
    auto actionMenuLight = new QAction("&Light", this);                                    // Switch to light
    connect(actionMenuLight, &QAction::triggered, this, &MainWindow::on_lightBtn_clicked); //Doesn't work.
    auto actionMenuDark = new QAction("&Dark", this);                                      //Switch to dark
    connect(actionMenuDark, &QAction::triggered, this, &MainWindow::on_darkBtn_clicked);   //Doesn't work.
    auto actionMenuToggle = new QAction("&Toggle Window", this);
    connect(actionMenuToggle, &QAction::triggered, this, &MainWindow::toggleVisibility);

    // Build tray items
    auto trayMenu = new QMenu(this);
    trayMenu->addAction(actionMenuToggle);
    trayMenu->addAction(actionMenuLight);
    trayMenu->addAction(actionMenuDark);
    trayMenu->addAction(actionMenuQuit);
    return trayMenu;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) // Define actions for SysTray L&M-click
{
    switch (reason) {
        case QSystemTrayIcon::Trigger: // Left-click to toggle window visibility
            toggleVisibility();
            break;

            //        case QSystemTrayIcon::MiddleClick: // Middle-click to toggle between light and dark
            //            utils.notify("Hello!", "You middle-clicked me", 0); // Must implement toggle
            //            break;

            // Must understand tray better - Why can't right click be part of switch statement?

        default: // Need to understand switch statements better - Why is this required?
            break;
    }
}

// Independent functions
void MainWindow::toggleVisibility() {
    if (this->isVisible() == 0) {
        this->setVisible(1);
        this->activateWindow();
    } else {
        this->setVisible(0);
    }
}

void MainWindow::schedule(Profile *const pProfile)
{
	settings->beginGroup("Favourites");
	auto favTime = QTime::fromString(settings->value(pProfile->name()).toString() );

	int cronMin = (favTime.minute() < 0) ? 0 : favTime.minute();
	int cronHr = (favTime.hour() < 0 ) ? 0: favTime.hour();


}
void MainWindow::scheduleLight() {
    int lightCronMin = QTime::fromString(utils.settings->value("time-light").toString()).minute();
    int lightCronHr = QTime::fromString(utils.settings->value("time-light").toString()).hour();
    if (lightCronMin <= 0) {
        lightCronMin = 0;
    }
    if (lightCronHr <= 0) {
        lightCronHr = 0;
    }
    std::string lightCron = std::to_string(lightCronMin) + " " + std::to_string(lightCronHr) + " * * *";
    s.cron(lightCron, [this]() {
        utils.go("light");
    });
}

void MainWindow::scheduleDark() {
    int darkCronMin = QTime::fromString(utils.settings->value("time-dark").toString()).minute();
    int darkCronHr = QTime::fromString(utils.settings->value("time-dark").toString()).hour();
    if (darkCronMin <= 0) {
        darkCronMin = 0;
    }
    if (darkCronHr <= 0) {
        darkCronHr = 0;
    }
    std::string darkCron = std::to_string(darkCronMin) + " " + std::to_string(darkCronHr) + " * * *";
    s.cron(darkCron, [this]() {
        utils.go("dark");
    });
}

// Functionality of buttons - Related to program navigation, interaction and saving settings
void MainWindow::on_prefsBtn_clicked() // Preferences button - Sets all preferences as found in koirc file
{
    auto *dialog = new SettingDialog(this);
    dialog->setModal(true);
    dialog->show();
}

void MainWindow::on_lightBtn_clicked() {
    utils.go("light");
}

void MainWindow::on_darkBtn_clicked() {
    utils.go("dark");
}
//widgetStyle

void MainWindow::on_scheduleRadioBtn_toggled(bool scheduleSun) // Toggle between manual schedule, and sun schedule
{
    if (ui->sunRadioBtn->isChecked() == 0) {
        ui->lightTimeLabel->setEnabled(1);
        ui->darkTimeLabel->setEnabled(1);
        ui->lightTimeEdit->setEnabled(1);
        ui->darkTimeEdit->setEnabled(1);
        scheduleType = "time";
        utils.settings->setValue("schedule-type", scheduleType);
        utils.settings->sync();
    } else {
        ui->lightTimeLabel->setEnabled(0);
        ui->darkTimeLabel->setEnabled(0);
        ui->lightTimeEdit->setEnabled(0);
        ui->darkTimeEdit->setEnabled(0);
        scheduleType = "sun";
        utils.settings->setValue("schedule-type", scheduleType);
        utils.settings->sync();
    }
}

void MainWindow::on_lightTimeEdit_userTimeChanged(const QTime &time) // Set light time
{
    lightTime = time.toString();
    utils.settings->setValue("time-light", lightTime);
    utils.settings->sync();
    ui->resMsg->setText(tr("Koi must be restarted for new times to be used."));
    ui->resMsg->setMessageType(KMessageWidget::Warning);
    ui->resMsg->animatedShow();
}

void MainWindow::on_darkTimeEdit_userTimeChanged(const QTime &time) // Set dark time
{
    darkTime = time.toString();
    utils.settings->setValue("time-dark", darkTime);
    utils.settings->sync();
    ui->resMsg->setText(tr("Koi must be restarted for new times to be used."));
    ui->resMsg->setMessageType(KMessageWidget::Warning);
    ui->resMsg->animatedShow();
}

void MainWindow::on_hiddenCheckBox_stateChanged(int hiddenEnabled) {
    ui->resMsg->animatedShow();
    if (ui->hiddenCheckBox->checkState() == 0) {
        utils.settings->setValue("start-hidden", false);
    } else {
        utils.settings->setValue("start-hidden", true);
    }
}

//void MainWindow::on_notifyCheckBox_stateChanged(int notifyEnabled) {
//    if (ui->notifyCheckBox->checkState() == 0) {
//        utils.settings->setValue("notify", false);
//    } else {
//        utils.settings->setValue("notify", true);
//    }
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
    this->setVisible(0);
}

void MainWindow::on_actionRestart_triggered()
{
	QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
	exit(12);
}

