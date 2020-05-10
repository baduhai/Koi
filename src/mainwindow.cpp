#include "headers/mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setIcon(QIcon(":/resources/icons/koi_tray.png")); // Set tray icon
    this->trayIcon->setVisible(true);
    trayMenu = this->createMenu();
    this->trayIcon->setContextMenu(trayMenu); // Set tray context menu
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated); // System tray interaction
    // Settings preperties used globally
    utils.settings = new QSettings("koirc", QSettings::IniFormat); // Line used for testing !Must comment before pushing!
    // utils.settings = new QSettings(QDir::homePath() + "/.config/koirc", QSettings::IniFormat); // Setting config path and format
    ui->setupUi(this);
    ui->mainStack->setCurrentIndex(0); // Always start window on main view
    refreshDirs();
    loadPrefs(); // Load prefs on startup
}
MainWindow::~MainWindow()
{
    this->setVisible(0);
}

// SysTray related functionality
QMenu* MainWindow::createMenu() // Define context menu items for SysTray - R-click to show context menu
{
    // Tray actions
    auto actionMenuQuit = new QAction("&Quit", this); // Quit app
    connect(actionMenuQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
    auto actionMenuLight = new QAction("&Light", this); // Switch to light
    //connect(actionMenuLight, &QAction::triggered, utils, &Utils::goLight); //Doesn't work.
    auto actionMenuDark = new QAction("&Dark", this); //Switch to dark
    //connect(actionMenuDark, &QAction::triggered, utils, &Utils::goDark); //Doesn't work.

    // Build tray items
    auto trayMenu = new QMenu(this);
    trayMenu->addAction(actionMenuLight);
    trayMenu->addAction(actionMenuDark);
    trayMenu->addSeparator();
    trayMenu->addAction(actionMenuQuit);
    return trayMenu;
}
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) // Define actions for SysTray L&M-click
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger: // Left-click to toggle window visibility
            toggleVisibility();
            break;

        case QSystemTrayIcon::MiddleClick: // Middle-click to toggle between light and dark
            this->trayIcon->showMessage("Hello", "You middle-clicked me!"); // Must implement toggle
            break;

        // Must understand tray better - Why can't right click be part of switch statement?

        default: // Need to understand switch statements better - Why is this required?
            break;
    }
}

// Independent funtions
void MainWindow::loadPrefs()
{
    // Load scheduling prefs
    if (utils.settings->value("schedule").toBool())
    {
        ui->autoCheckBox->setChecked(1);
    }
    else
    {
        ui->autoCheckBox->setChecked(0);
    }
    if (utils.settings->value("schedule-type") == "time")
    {
        ui->scheduleRadioBtn->setChecked(1);
    }
    else
    {
        ui->sunRadioBtn->setChecked(1);
    }
    ui->lightTimeEdit->setTime(utils.settings->value("time-light").toTime());
    ui->darkTimeEdit->setTime(utils.settings->value("time-dark").toTime());

    // Load Plasma style prefs
    if (utils.settings->value("PlasmaStyle/enabled").toBool())
    {
        ui->styleCheckBox->setChecked(true);
    }
    else
    {
        ui->styleCheckBox->setChecked(false);
    }
    ui->lightDropStyle->setCurrentText(utils.settings->value("PlasmaStyle/dark").toString());
    ui->darkDropStyle->setCurrentText(utils.settings->value("PlasmaTheming/dark").toString());

    // Load color scheme prefs
    if (utils.settings->value("ColorScheme/enabled").toBool())
    {
        ui->colorCheckBox->setChecked(true);
    }
    else
    {
        ui->colorCheckBox->setChecked(false);
    }
    ui->lightDropColor->setCurrentText(utils.settings->value("ColorScheme/light").toString());
    ui->darkDropColor->setCurrentText(utils.settings->value("ColorScheme/dark").toString());

    // Load icon theme prefs
    if (utils.settings->value("IconTheme/enabled").toBool())
    {
        ui->iconCheckBox->setChecked(true);
    }
    else
    {
        ui->iconCheckBox->setChecked(false);
    }
    ui->lightDropIcon->setCurrentText(utils.settings->value("IconTheme/light").toString());
    ui->darkDropIcon->setCurrentText(utils.settings->value("IconTheme/dark").toString());

    // Load GTK Theme prefs
    if (utils.settings->value("GTKTheme/enabled").toBool())
    {
        ui->gtkCheckBox->setChecked(true);
    }
    else
    {
        ui->gtkCheckBox->setChecked(false);
    }
    ui->lightDropGtk->setCurrentText(utils.settings->value("GTKTheme/light").toString());
    ui->darkDropGtk->setCurrentText(utils.settings->value("GTKTheme/dark").toString());

    // Load Wallpaper prefs
    if (utils.settings->value("Wallpaper/enabled").toBool())
    {
        ui->wallCheckBox->setChecked(true);
    }
    else
    {
        ui->wallCheckBox->setChecked(false);
    }
    QFileInfo lw(utils.settings->value("Wallpaper/light").toString());
    QString lightWallBtnText = lw.fileName();
    if (lightWall.isEmpty())
    {
        ui->lightWallBtn->setText("Select...");
    }
    else
    {
        ui->lightWallBtn->setText(lightWallBtnText);
    }
    QFileInfo dw(utils.settings->value("Wallpaper/dark").toString());
    QString darkWallBtnText = dw.fileName();
    if (darkWall.isEmpty())
    {
        ui->darkWallBtn->setText("Select...");
    }
    else
    {
        ui->darkWallBtn->setText(darkWallBtnText);
    }
}
void MainWindow::savePrefs()
{
    // Plasma Style enabling
    if (ui->styleCheckBox->isChecked() == 0)
    {
        utils.settings->setValue("PlasmaStyle/enabled", false);
    }
    else
    {
        utils.settings->setValue("PlasmaStyle/enabled", true);
    }
    // Plasma Style saving prefs
    utils.settings->setValue("PlasmaStyle/light", lightStyle);
    utils.settings->setValue("PlasmaStyle/dark", darkStyle);

    // Color scheme enabling
    if (ui->colorCheckBox->isChecked() == 0)
    {
        utils.settings->setValue("ColorScheme/enabled", false);
    }
    else
    {
        utils.settings->setValue("ColorScheme/enabled", true);
    }
    // Color scheme saving prefs
    utils.settings->setValue("ColorScheme/light", lightColor);
    utils.settings->setValue("ColorScheme/dark", darkColor);

    // Icon theme enabling
    if (ui->iconCheckBox->checkState() == 0)
    {
        utils.settings->setValue("IconTheme/enabled", false);
    }
    else
    {
        utils.settings->setValue("IconTheme/enabled", true);
    }
    // Icon theme saving prefs
    utils.settings->setValue("IconTheme/light", lightIcon);
    utils.settings->setValue("IconTheme/dark", darkIcon);

    // GTK theme enabling
    if (ui->gtkCheckBox->isChecked() == 0)
    {
        utils.settings->setValue("GTKTheme/enabled", false);
    }
    else
    {
        utils.settings->setValue("GTKTheme/enabled", true);
    }
    // GTK theme saving prefs
    utils.settings->setValue("GTKTheme/light", lightGtk);
    utils.settings->setValue("GTKTheme/dark", darkGtk);

    // Wallpaper enabling
    if (ui->wallCheckBox->isChecked() == 0)
    {
        utils.settings->setValue("Wallpaper/enabled", false);
    }
    else
    {
        utils.settings->setValue("Wallpaper/enabled", true);
    }
    // Wallpaper saving prefs
    utils.settings->setValue("Wallpaper/light", lightWall);
    utils.settings->setValue("Wallpaper/dark", darkWall);
    utils.settings->sync();
}
void MainWindow::refreshDirs() // Refresh function to find new themes
{
    // Refresh plasma styles
    QStringList plasmaStyles = utils.getPlasmaStyles();
    ui->lightDropStyle->clear();
    ui->lightDropStyle->addItems(plasmaStyles);
    ui->darkDropStyle->clear();
    ui->darkDropStyle->addItems(plasmaStyles);
    // Refresh color schemes
    QStringList colorSchemes = utils.getColorSchemes();
    ui->lightDropColor->clear();
    ui->lightDropColor->addItems(colorSchemes);
    ui->darkDropColor->clear();
    ui->darkDropColor->addItems(colorSchemes);
    // Refresh icon themes
    QStringList iconThemes = utils.getIconThemes();
    ui->lightDropIcon->clear();
    ui->lightDropIcon->addItems(iconThemes);
    ui-> darkDropIcon->clear();
    ui-> darkDropIcon->addItems(iconThemes);
    // Refresh gtk themes
    QStringList gtkThemes = utils.getGtkThemes();
    ui->lightDropGtk->clear();
    ui->lightDropGtk->addItems(gtkThemes);
    ui->darkDropGtk->clear();
    ui->darkDropGtk->addItems(gtkThemes);
    loadPrefs();
}
void MainWindow::toggleVisibility()
{
    if (this->isVisible() == 0)
    {
        this->setVisible(1);
    }
    else
    {
        this->setVisible(0);
    }
}

// Funtionality of buttons - Related to program navigation, interaction and saving settings
void MainWindow::on_prefsBtn_clicked() // Preferences button - Sets all preferences as found in koirc file
{
    lightWall = utils.settings->value("Wallpaper/light").toString();
    darkWall = utils.settings->value("Wallpaper/dark").toString();
    /*
     * The two lines above fix a bug where when applying the settings without having changed
     * the wallpapers, the wallpaper preferences would be set as empty strings, and not stay
     * the same, as is expected behaviour. Unsure why this fixes said bug... ¯\_(ツ)_/¯
     */
    loadPrefs();
    ui->mainStack->setCurrentIndex(1);
}
void MainWindow::on_backBtn_clicked() // Back button in preferences view - Must setup cheking if prefs saved
{
    ui->mainStack->setCurrentIndex(0);
}
void MainWindow::on_applyBtn_clicked()
{
    savePrefs();
}
void MainWindow::on_refreshBtn_clicked() // Refresh dirs contents
{
    loadPrefs();
    refreshDirs();
}

// Editing options
void MainWindow::on_styleCheckBox_stateChanged(int styleEnabled) // Plasma style checkbox logic
{
    if (ui->styleCheckBox->checkState() == 0)
    {
        ui->darkStyle->setEnabled(0);
        ui->lightStyle->setEnabled(0);
        ui->darkDropStyle->setEnabled(0);
        ui->lightDropStyle->setEnabled(0);
    }
    else
    {
        ui->darkStyle->setEnabled(1);
        ui->lightStyle->setEnabled(1);
        ui->darkDropStyle->setEnabled(1);
        ui->lightDropStyle->setEnabled(1);
    }

}
void MainWindow::on_colorCheckBox_stateChanged(int colorEnabled) // Color scheme checkbox logic
{
    if (ui->colorCheckBox->checkState() == 0)
    {
        ui->darkColor->setEnabled(0);
        ui->lightColor->setEnabled(0);
        ui->darkDropColor->setEnabled(0);
        ui->lightDropColor->setEnabled(0);
    }
    else
    {
        ui->darkColor->setEnabled(1);
        ui->lightColor->setEnabled(1);
        ui->darkDropColor->setEnabled(1);
        ui->lightDropColor->setEnabled(1);
    }
}
void MainWindow::on_iconCheckBox_stateChanged(int iconEnabled) // Icon theme checkbox logic
{
    if (ui->iconCheckBox->checkState() == 0)
    {
        ui->darkIcon->setEnabled(0);
        ui->lightIcon->setEnabled(0);
        ui->darkDropIcon->setEnabled(0);
        ui->lightDropIcon->setEnabled(0);
    }
    else
    {
        ui->darkIcon->setEnabled(1);
        ui->lightIcon->setEnabled(1);
        ui->darkDropIcon->setEnabled(1);
        ui->lightDropIcon->setEnabled(1);
    }
}
void MainWindow::on_lightDropStyle_currentIndexChanged(const QString &lightStyleUN) // Set light plasma style
{
    lightStyle = lightStyleUN;
}
void MainWindow::on_darkDropStyle_currentIndexChanged(const QString &darkStyleUN) // Set dark plasma style
{
    darkStyle = darkStyleUN;
}
void MainWindow::on_lightDropColor_currentIndexChanged(const QString &lightColorUN) // Set light color scheme
{
    lightColor = lightColorUN;
}
void MainWindow::on_darkDropColor_currentIndexChanged(const QString &darkColorUN) // Set dark color scheme
{
    darkColor = darkColorUN;
}
void MainWindow::on_lightDropIcon_currentIndexChanged(const QString &lightIconUN) // Set light icon theme
{
    lightIcon = lightIconUN;
}
void MainWindow::on_darkDropIcon_currentIndexChanged(const QString &darkIconUN) // Set dark icon theme
{
    darkIcon = darkIconUN;
}
void MainWindow::on_gtkCheckBox_stateChanged(int gtkEnlabled) // GTK theme checkbox logic
{
    if (ui->gtkCheckBox->checkState() == 0)
    {
        ui->darkGtk->setEnabled(0);
        ui->lightGtk->setEnabled(0);
        ui->darkDropGtk->setEnabled(0);
        ui->lightDropGtk->setEnabled(0);
    }
    else
    {
        ui->darkGtk->setEnabled(1);
        ui->lightGtk->setEnabled(1);
        ui->darkDropGtk->setEnabled(1);
        ui->lightDropGtk->setEnabled(1);
    }
}
void MainWindow::on_lightDropGtk_currentIndexChanged(const QString &lightGtkUN) // Set light gtk theme
{
    lightGtk = lightGtkUN;
}
void MainWindow::on_darkDropGtk_currentIndexChanged(const QString &darkGtkUN) // Set dark gtk theme
{
    darkGtk = darkGtkUN;
}
void MainWindow::on_wallCheckBox_stateChanged(int wallEnabled) // Wallpaper checkbox logic
{
    if (ui->wallCheckBox->checkState() == 0)
    {
        ui->darkWall->setEnabled(0);
        ui->lightWall->setEnabled(0);
        ui->darkWallBtn->setEnabled(0);
        ui->lightWallBtn->setEnabled(0);
    }
    else
    {
        ui->darkWall->setEnabled(1);
        ui->lightWall->setEnabled(1);
        ui->darkWallBtn->setEnabled(1);
        ui->lightWallBtn->setEnabled(1);
    }
}
void MainWindow::on_lightWallBtn_clicked() // Set light wallpaper
{
    lightWall = QFileDialog::getOpenFileName(this, tr("Select Image"), QDir::homePath() + "/Pictures", tr("Images(*.png *.jpg *.jpeg *.bmp)"));
    QFileInfo lw(lightWall);
    QString lightWallName = lw.fileName();
    ui->lightWallBtn->setText(lightWallName);
    ui->lightWallBtn->setToolTip(lightWall);
}
void MainWindow::on_darkWallBtn_clicked() // Set dark wallpaper
{
    darkWall = QFileDialog::getOpenFileName(this, tr("Select Image"), QDir::homePath() + "/Pictures", tr("Images(*.png *.jpg *.jpeg *.bmp)"));
    QFileInfo dw(darkWall);
    QString darkWallName = dw.fileName();
    ui->darkWallBtn->setText(darkWallName);
    ui->darkWallBtn->setToolTip(darkWall);
}
void MainWindow::on_autoCheckBox_stateChanged(int automaticEnabled) // Logic for enabling scheduling of themes
{
    if (ui->autoCheckBox->checkState() == 0)
    {
        ui->scheduleRadioBtn->setEnabled(0);
        ui->sunRadioBtn->setEnabled(0);
        ui->lightTimeLabel->setEnabled(0);
        ui->darkTimeLabel->setEnabled(0);
        ui->lightTimeEdit->setEnabled(0);
        ui->darkTimeEdit->setEnabled(0);
        utils.settings->setValue("schedule", false);
        utils.settings->sync();
    }
    else
    {

        ui->scheduleRadioBtn->setEnabled(1);
        ui->sunRadioBtn->setEnabled(1);
        ui->lightTimeLabel->setEnabled(1);
        ui->darkTimeLabel->setEnabled(1);
        ui->lightTimeEdit->setEnabled(1);
        ui->darkTimeEdit->setEnabled(1);
        if (ui->scheduleRadioBtn->isChecked() == 0)
        {
            ui->lightTimeLabel->setEnabled(0);
            ui->darkTimeLabel->setEnabled(0);
            ui->lightTimeEdit->setEnabled(0);
            ui->darkTimeEdit->setEnabled(0);
        }
        utils.settings->setValue("schedule", true);
        utils.settings->sync();
    }
}
void MainWindow::on_scheduleRadioBtn_toggled(bool scheduleSun) // Toggle between manual schedule, and sun schedule
{
    if (ui->sunRadioBtn->isChecked() == 0)
    {
        ui->lightTimeLabel->setEnabled(1);
        ui->darkTimeLabel->setEnabled(1);
        ui->lightTimeEdit->setEnabled(1);
        ui->darkTimeEdit->setEnabled(1);
        scheduleType = "time";
        utils.settings->setValue("schedule-type", scheduleType);
        utils.settings->sync();
    }
    else
    {
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
}
void MainWindow::on_darkTimeEdit_userTimeChanged(const QTime &time) // Set dark time
{
    darkTime = time.toString();
    utils.settings->setValue("time-dark", darkTime);
    utils.settings->sync();
}

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
    auto* about = new About(this);
    about->open();
}
void MainWindow::on_actionHide_triggered() // Hide to tray
{
    this->setVisible(0);
}
void MainWindow::on_actionRefresh_triggered() // Refresh dirs
{
    on_refreshBtn_clicked();
}




void MainWindow::on_lightBtn_clicked()
{
    utils.goLight();
}

void MainWindow::on_darkBtn_clicked()
{
    utils.goDark();
}
