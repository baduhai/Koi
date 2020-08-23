#include "mainwindow.h"
#include "ui_mainwindow.h"

Bosma::Scheduler s(2);

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setIcon(QIcon(":/resources/icons/koi_tray.png")); // Set tray icon - Not sure why svg doesn't work
    this->trayIcon->setVisible(true);
    trayMenu = this->createMenu();
    this->trayIcon->setContextMenu(trayMenu);                                         // Set tray context menu
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated); // System tray interaction
    utils.initialiseSettings();
    ui->setupUi(this);
    ui->mainStack->setCurrentIndex(0); // Always start window on main view
    refreshDirs();
    loadPrefs(); // Load prefs on startup
    if (utils.settings->value("schedule").toBool()) {
        utils.startupTimeCheck(); // Switch themes on startup
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
void MainWindow::loadPrefs() {
    // Load notify prefs
    if (utils.settings->value("notify", true).toBool()) {
        ui->notifyCheckBox->setChecked(true);
    } else {
        ui->notifyCheckBox->setChecked(false);
    }
    // Load startup prefs
    if (utils.settings->value("start-hidden").toBool()) {
        ui->hiddenCheckBox->setChecked(true);
    } else {
        ui->hiddenCheckBox->setChecked(false);
    }

    // Load scheduling prefs
    if (utils.settings->value("schedule").toBool()) {
        ui->autoCheckBox->setChecked(1);
    } else {
        ui->autoCheckBox->setChecked(0);
    }
    if (utils.settings->value("schedule-type") == "time") {
        ui->scheduleRadioBtn->setChecked(1);
    } else {
        ui->sunRadioBtn->setChecked(1);
    }
    ui->lightTimeEdit->setTime(utils.settings->value("time-light").toTime());
    ui->darkTimeEdit->setTime(utils.settings->value("time-dark").toTime());

    // Load Plasma style prefs
    if (utils.settings->value("PlasmaStyle/enabled").toBool()) {
        ui->styleCheckBox->setChecked(true);
    } else {
        ui->styleCheckBox->setChecked(false);
    }
    ui->lightDropStyle->setCurrentText(utils.settings->value("PlasmaStyle/light").toString());
    ui->darkDropStyle->setCurrentText(utils.settings->value("PlasmaStyle/dark").toString());

    // Load color scheme prefs
    if (utils.settings->value("ColorScheme/enabled").toBool()) {
        ui->colorCheckBox->setChecked(true);
    } else {
        ui->colorCheckBox->setChecked(false);
    }
    QFileInfo lightColorsPref(utils.settings->value("ColorScheme/light").toString());
    QFileInfo darkColorsPref(utils.settings->value("ColorScheme/dark").toString());
    QString lightColorsPrefString = lightColorsPref.baseName();
    QString darkColorsPrefString = darkColorsPref.baseName();
    ui->lightDropColor->setCurrentText(lightColorsPrefString);
    ui->darkDropColor->setCurrentText(darkColorsPrefString);

    // Load icon theme prefs
    if (utils.settings->value("IconTheme/enabled").toBool()) {
        ui->iconCheckBox->setChecked(true);
    } else {
        ui->iconCheckBox->setChecked(false);
    }
    ui->lightDropIcon->setCurrentText(utils.settings->value("IconTheme/light").toString());
    ui->darkDropIcon->setCurrentText(utils.settings->value("IconTheme/dark").toString());

    // Load GTK Theme prefs
    if (utils.settings->value("GTKTheme/enabled").toBool()) {
        ui->gtkCheckBox->setChecked(true);
    } else {
        ui->gtkCheckBox->setChecked(false);
    }
    ui->lightDropGtk->setCurrentText(utils.settings->value("GTKTheme/light").toString());
    ui->darkDropGtk->setCurrentText(utils.settings->value("GTKTheme/dark").toString());

    ui->lightDropWidget->setCurrentText(utils.settings->value("WidgetStyle/light").toString());
    ui->darkDropWidget->setCurrentText(utils.settings->value("WidgetStyle/dark").toString());
    // Load Kvantum Style theme prefs
    // sets the displayed text on the combo box of the kvantum style.
    ui->lightDropKvantumStyle->setCurrentText(utils.settings->value("KvantumStyle/light").toString());
    ui->darkDropKvantumStyle->setCurrentText(utils.settings->value("KvantumStyle/Dark").toString());

    //load cursor preferences 
    ui->lightDropCursor->setCurrentText(utils.settings->value("Mouse/light").toString());
    ui->darkDropCursor->setCurrentText(utils.settings->value("Mouse/dark").toString());

    ui->lightDropDecoration->setCurrentText(utils.settings->value("WindowDecoration/light").toString());
    ui->darkDropDecoration->setCurrentText(utils.settings->value("WindowDecoration/light").toString());

    // Load Wallpaper prefs
    if (utils.settings->value("Wallpaper/enabled").toBool()) {
        ui->wallCheckBox->setChecked(true);
    } else {
        ui->wallCheckBox->setChecked(false);
    }
    QFileInfo lw(utils.settings->value("Wallpaper/light").toString());
    QString lightWallBtnText = lw.fileName();
    if (lightWall.isEmpty()) {
        ui->lightWallBtn->setText("Select...");
    } else {
        ui->lightWallBtn->setText(lightWallBtnText);
    }
    QFileInfo dw(utils.settings->value("Wallpaper/dark").toString());
    QString darkWallBtnText = dw.fileName();
    if (darkWall.isEmpty()) {
        ui->darkWallBtn->setText("Select...");
    } else {
        ui->darkWallBtn->setText(darkWallBtnText);
    }

    if (utils.settings->value("Script/lightEnabled").toBool()) {
        ui->lightScriptCheckBox->setChecked(true);
    } else {
        ui->lightScriptCheckBox->setChecked(false);
    }

    if (utils.settings->value("Script/darkEnabled").toBool()) {
        ui->darkScriptCheckBox->setChecked(true);
    } else {
        ui->darkScriptCheckBox->setChecked(false);
    }
    //Run Script Preferences
    QFileInfo scriptL(utils.settings->value("Script/light").toString());
    QString lightScriptBtnText = scriptL.fileName();
    if(lightScript.isEmpty()){
    	ui->lightScriptBtn->setText("Select Light Script");
    }else {
    	ui->lightScriptBtn->setText(lightScriptBtnText);
    }

    QFileInfo scriptD(utils.settings->value("Script/dark").toString());
    QString darkScriptBtnText = scriptD.fileName();
    if(darkScript.isEmpty()){
    	ui->darkScriptBtn->setText("Select Dark Script");
    }else {
    	ui->darkScriptBtn->setText(darkScriptBtnText);
    }

}

void MainWindow::savePrefs() {

    //make this asynchronous
    //TODO remove checkboxes and its values because you technically cannot disable the theme for some styles.
    //Todo and they are redundant ,also in util.settings .

    // Plasma Style enabling
    if (ui->styleCheckBox->isChecked() == 0) {
        utils.settings->setValue("PlasmaStyle/enabled", false);
    } else {
        utils.settings->setValue("PlasmaStyle/enabled", true);
    }
    // Plasma Style saving prefs
    utils.settings->setValue("PlasmaStyle/light", lightStyle);
    utils.settings->setValue("PlasmaStyle/dark", darkStyle);

    // Color scheme enabling
    if (ui->colorCheckBox->isChecked() == 0) {
        utils.settings->setValue("ColorScheme/enabled", false);
    } else {
        utils.settings->setValue("ColorScheme/enabled", true);
    }
    // Color scheme saving prefs
    utils.settings->setValue("ColorScheme/light", lightColor);
    utils.settings->setValue("ColorScheme/dark", darkColor);

    // Icon theme enabling
    if (ui->iconCheckBox->checkState() == 0) {
        utils.settings->setValue("IconTheme/enabled", false);
    } else {
        utils.settings->setValue("IconTheme/enabled", true);
    }
    // Icon theme saving prefs
    utils.settings->setValue("IconTheme/light", lightIcon);
    utils.settings->setValue("IconTheme/dark", darkIcon);

    // GTK theme enabling
    if (ui->gtkCheckBox->isChecked() == 0) {
        utils.settings->setValue("GTKTheme/enabled", false);
    } else {
        utils.settings->setValue("GTKTheme/enabled", true);
    }
    // GTK theme saving prefs
    utils.settings->setValue("GTKTheme/light", lightGtk);
    utils.settings->setValue("GTKTheme/dark", darkGtk);

    //Kvantum Style Theme saving Prefs
    utils.settings->setValue("KvantumStyle/light", lightKvantumStyle);
    utils.settings->setValue("KvantumStyle/dark", darkKvantumStyle);

    // Wallpaper enabling
    if (ui->wallCheckBox->isChecked() == 0) {
        utils.settings->setValue("Wallpaper/enabled", false);
    } else {
        utils.settings->setValue("Wallpaper/enabled", true);
    }
    // Wallpaper saving prefs
    utils.settings->setValue("Wallpaper/light", lightWall);
    utils.settings->setValue("Wallpaper/dark", darkWall);

    // Run Scripts
    //light
    if(ui->lightScriptCheckBox->isChecked() == 0 )
	{
    	utils.settings->setValue("Script/lightEnabled", false);
	}else {
    	utils.settings->setValue("Script/lightEnabled", true);
    }


    //dark script
    if(ui->darkScriptBtn->isChecked() == 0 )
	{
    	utils.settings->setValue("Script/darkEnabled", false);
	}else {
    	utils.settings->setValue("Script/darkEnabled", true);
    }
    utils.settings->setValue("Script/light",lightScript);
    utils.settings->setValue("Script/dark",darkScript);

    //Cursor Style Theme saving Prefs
    utils.settings->setValue(QStringLiteral("Mouse/light"), lightCursor);
    utils.settings->setValue(QStringLiteral("Mouse/dark"), darkCursor);

    //widget style theme saving prefs
    utils.settings->setValue(QStringLiteral("WidgetStyle/light") ,lightWidget);
    utils.settings->setValue(QStringLiteral("WidgetStyle/dark"), darkWidget);
    //this would write to the actual lookand feel folder of the theme

    //Decoration Style theme saving prefs
    //TODO break out of the for loop once found.
    QList<Decoration> decList= utils.getWindowDecorations();
    for (const auto &dt : qAsConst(decList)){
        if (QString::compare(dt.name, darkDecoration, Qt::CaseInsensitive) == 0){
            darkDecorationLibrary = dt.library;
            darkDecorationTheme = dt.theme;
        }
    }
    for (const auto &dt : qAsConst(decList)){
        if (QString::compare(dt.name, lightDecoration, Qt::CaseInsensitive) == 0){
            lightDecorationLibrary = dt.library;
            lightDecorationTheme = dt.theme;
        }
    }
    utils.settings->setValue(QStringLiteral("WindowDecoration/light"),lightDecoration);
    utils.settings->setValue(QStringLiteral("WindowDecoration/lightLibrary"),lightDecorationLibrary);
    utils.settings->setValue(QStringLiteral("WindowDecoration/lightTheme"),lightDecorationTheme);
    utils.settings->setValue(QStringLiteral("WindowDecoration/dark"),darkDecoration);
    utils.settings->setValue(QStringLiteral("WindowDecoration/darkLibrary"),darkDecorationLibrary);
    utils.settings->setValue(QStringLiteral("WindowDecoration/darkTheme"),darkDecorationTheme);

    utils.settings->sync();
}

void MainWindow::refreshDirs() // Refresh function to find new themes
{
    // Refresh plasma styles
    QStringList plasmaStyles = PlasmaStyle::getPlasmaStyles();
    ui->lightDropStyle->clear();
    ui->lightDropStyle->addItems(plasmaStyles);
    ui->darkDropStyle->clear();
    ui->darkDropStyle->addItems(plasmaStyles);
    // Refresh color schemes
    QStringList colorSchemes = ColorScheme::getColorSchemes();
    ui->lightDropColor->clear();
    ui->lightDropColor->addItems(colorSchemes);
    ui->darkDropColor->clear();
    ui->darkDropColor->addItems(colorSchemes);
    // Refresh icon themes
    QStringList iconThemes = Icons::getIconThemes();
    ui->lightDropIcon->clear();
    ui->lightDropIcon->addItems(iconThemes);
    ui->darkDropIcon->clear();
    ui->darkDropIcon->addItems(iconThemes);
    // Refresh gtk themes
    QStringList gtkThemes = Gtk::getGtkThemes();
    ui->lightDropGtk->clear();
    ui->lightDropGtk->addItems(gtkThemes);
    ui->darkDropGtk->clear();
    ui->darkDropGtk->addItems(gtkThemes);
    // Refresh Kvantum Style themes.
    QStringList kvantumStyle = KvantumStyle::getKvantumStyles();
    ui->lightDropKvantumStyle->clear(); //clears everything from the kvantum drop down menu
    ui->darkDropKvantumStyle->clear();
    ui->lightDropKvantumStyle->addItems(kvantumStyle); //adds the new loaded kvantum styles
    ui->darkDropKvantumStyle->addItems(kvantumStyle);

    //cursor themes.
    QStringList cursorTheme = Utils::getCursorThemes();
    ui->lightDropCursor->clear();
    ui->darkDropCursor->clear();
    ui->lightDropCursor->addItems(cursorTheme);
    ui->darkDropCursor->addItems(cursorTheme);
    //widget styles.
    QStringList widgetStyle = Utils::getWidgetStyles();
    ui->lightDropWidget->clear();
    ui->darkDropWidget->clear();
    ui->lightDropWidget->addItems(widgetStyle);
    ui->darkDropWidget->addItems(widgetStyle);
    //Window decoration styles
    QStringList decorationStyle = Utils::getWindowDecorationsStyle();
    ui->lightDropDecoration->clear();
    ui->darkDropDecoration->clear();
    ui->lightDropDecoration->addItems(decorationStyle);
    ui->darkDropDecoration->addItems(decorationStyle);

    loadPrefs();
}

void MainWindow::toggleVisibility() {
    if (this->isVisible() == 0) {
        this->setVisible(1);
        this->activateWindow();
    } else {
        this->setVisible(0);
    }
}

int MainWindow::prefsSaved() // Lots of ifs, don't know how to do it any other way. Maybe an array?
{
    //Todo simplyfy this probably by creating an emit change if the index of the combo boxes changes
    //TODO add missing preferences here
    //by using signals

    //to choose the settings page
        if (ui->styleCheckBox->isChecked() != utils.settings->value("PlasmaStyle/enabled").toBool()) {
            return 0;
        }
        if (lightStyle != utils.settings->value("PlasmaStyle/light").toString()) {
            return 0;
        }
        if (darkStyle != utils.settings->value("PlasmaStyle/dark").toString()) {
            return 0;
        }
        if (ui->colorCheckBox->isChecked() != utils.settings->value("ColorScheme/enabled").toBool()) {
            return 0;
        }
        if (lightColor != utils.settings->value("ColorScheme/light").toString()) {
            return 0;
        }
        if (darkColor != utils.settings->value("ColorScheme/dark").toString()) {
            return 0;
        }
        if (ui->iconCheckBox->isChecked() != utils.settings->value("IconTheme/enabled").toBool()) {
            return 0;
        }
        if (lightIcon != utils.settings->value("IconTheme/light").toString()) {
            return 0;
        }
        if (darkIcon != utils.settings->value("IconTheme/dark").toString()) {
            return 0;
        }
        if (ui->gtkCheckBox->isChecked() != utils.settings->value("GTKTheme/enabled").toBool()) {
            return 0;
        }
        if (lightGtk != utils.settings->value("GTKTheme/light").toString()) {
            return 0;
        }
        if (darkGtk != utils.settings->value("GTKTheme/dark").toString()) {
            return 0;
        }
        if (ui->wallCheckBox->isChecked() != utils.settings->value("Wallpaper/enabled").toBool()) {
            return 0;
        }
        if (lightWall != utils.settings->value("Wallpaper/light").toString()) {
            return 0;
        }
        if (darkWall != utils.settings->value("Wallpaper/dark").toString()) {
            return 0;
        }
        if (lightKvantumStyle != utils.settings->value("KvantumStyle/light").toString()) {
            return 0;
        }
        if (darkKvantumStyle != utils.settings->value("KvantumStyle/dark").toString()) {
            return 0;
        }
        if (lightWidget != utils.settings->value("WidgetStyle/light").toString())   {
            return 0;
        }
        if (darkWidget != utils.settings->value("WidgetStyle/dark").toString()){
            return 0;
        }
        if (darkCursor != utils.settings->value("Mouse/dark").toString()){
            return 0;
        }
        if (lightCursor != utils.settings->value("Mouse/light").toString() ){
            return  0;
        }
        if (lightDecoration != utils.settings->value("WidgetDecoration/light").toString()){
            return 0;
        }
        if (darkDecoration != utils.settings->value("WidgetDecoration/dark").toString()){
            return 0;
        }

    return 1;
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

void MainWindow::on_backBtn_clicked() // Back button in preferences view - Must setup checking if prefs saved
{
    if (prefsSaved()) {
        ui->mainStack->setCurrentIndex(0);
    } else {
        QMessageBox applyConfs; // Verify if user wants to save settings
        applyConfs.setWindowTitle("Save Settings — Koi");
        applyConfs.setText("You have unsaved changes, would you like to save or discard them?");
        applyConfs.setIcon(QMessageBox::Warning);
        applyConfs.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        applyConfs.setDefaultButton(QMessageBox::Save);
        int ret = applyConfs.exec();
        switch (ret) {
            case QMessageBox::Save: // Save and change stack
                savePrefs();
                ui->mainStack->setCurrentIndex(0);
                break;
            case QMessageBox::Discard: // Change stack
                ui->mainStack->setCurrentIndex(0);
                lightWall = utils.settings->value("Wallpaper/light").toString();
                darkWall = utils.settings->value("Wallpaper/dark").toString();
                loadPrefs();
                break;
            case QMessageBox::Cancel: // Do nothin  //probably dont need this case
                break;
            default:
                break;
        }
    }
}

void MainWindow::on_advancedPageBtn_clicked() {
    ui->mainStack->setCurrentIndex(2);
}

void MainWindow::on_advancedPageBackBtn_clicked() {
    ui->mainStack->setCurrentIndex(1);
}

void MainWindow::on_advancedPageHomeBtn_clicked() {
    ui->mainStack->setCurrentIndex(0);
}

void MainWindow::on_applyBtn_clicked() {
    savePrefs();
    //dont know why but when it is in saveprefs; it requires two clicks on
    //apply button it works fine here
    Utils::writeToThemeConfigFile("Koi-light", "light");
    Utils::writeToThemeConfigFile("Koi-dark", "dark");
}
//Todo cursors on the desktop don't update you need to restart the plasma shell
// it also doesn't even work in the normal kde systemsettings
void MainWindow::on_lightBtn_clicked() {
    utils.go("light");
}

void MainWindow::on_darkBtn_clicked() {
    utils.go("dark");
}
//widgetStyle
void MainWindow::on_lightDropWidget_currentIndexChanged(const QString &lightWidgetUN){
    lightWidget= lightWidgetUN;
    if(lightWidgetUN == "kvantum" || lightWidgetUN == "kvantum-dark"){
        ui->lightKvantumStyle->setEnabled(true);
        ui->lightDropKvantumStyle->setEnabled(true);
    } else {
        ui->lightKvantumStyle->setEnabled(false);
        ui->lightDropKvantumStyle->setEnabled(false);
    }
}

//runcommand dark
void MainWindow::on_lightScriptCheckBox_stateChanged(int lightScriptEnabled){
    if (lightScriptEnabled == false) {
    	ui->lightScriptBtn->setEnabled(false);
    } else {
    	ui->lightScriptBtn->setEnabled(true);
    }
}

//runcommand dark
void MainWindow::on_darkScriptCheckBox_stateChanged(int darkScriptEnabled){
    if ( darkScriptEnabled == false) {
    	ui->darkScriptBtn->setEnabled(false);
    } else {
    	ui->darkScriptBtn->setEnabled(true);
    }
}
void MainWindow::on_darkDropWidget_currentIndexChanged(const QString &darkWidgetUN) {
    darkWidget = darkWidgetUN;
    if(darkWidgetUN == "kvantum" || darkWidgetUN == "kvantum-dark"){
        ui->darkKvantumStyle->setEnabled(true);
        ui->darkDropKvantumStyle->setEnabled(true);
    } else {
        ui->darkKvantumStyle->setEnabled(false);
        ui->darkDropKvantumStyle->setEnabled(false);
    }
}
//CursorStyle
void MainWindow::on_lightDropCursor_currentIndexChanged(const QString &lightCursorUN) {
    lightCursor = lightCursorUN;
}

void MainWindow::on_darkDropCursor_currentIndexChanged(const QString &darkCursorUN) {
    darkCursor = darkCursorUN;

}

//Decoration Style
void MainWindow::on_lightDropDecoration_currentIndexChanged(const QString &lightDecorationUN){
    lightDecoration = lightDecorationUN;
    QList<Decoration> decList= utils.getWindowDecorations();
    for (const auto &dt : qAsConst(decList)){
        if (QString::compare(dt.name, darkColor, Qt::CaseInsensitive) == 0){
            lightDecorationLibrary = dt.library;
            lightDecorationTheme = dt.theme;
        }
    }
}

void MainWindow::on_darkDropDecoration_currentIndexChanged(const QString &darkDecorationUN){
    darkDecoration = darkDecorationUN;
}

void MainWindow::on_styleCheckBox_stateChanged(int styleEnabled) // Plasma style checkbox logic
{
    if (ui->styleCheckBox->checkState() == 0) {
        ui->darkStyle->setEnabled(0);
        ui->lightStyle->setEnabled(0);
        ui->darkDropStyle->setEnabled(0);
        ui->lightDropStyle->setEnabled(0);
    } else {
        ui->darkStyle->setEnabled(1);
        ui->lightStyle->setEnabled(1);
        ui->darkDropStyle->setEnabled(1);
        ui->lightDropStyle->setEnabled(1);
    }
}

void MainWindow::on_colorCheckBox_stateChanged(int colorEnabled) // Color scheme checkbox logic
{
    if (ui->colorCheckBox->checkState() == 0) {
        ui->darkColor->setEnabled(0);
        ui->lightColor->setEnabled(0);
        ui->darkDropColor->setEnabled(0);
        ui->lightDropColor->setEnabled(0);
    } else {
        ui->darkColor->setEnabled(1);
        ui->lightColor->setEnabled(1);
        ui->darkDropColor->setEnabled(1);
        ui->lightDropColor->setEnabled(1);
    }
}

void MainWindow::on_iconCheckBox_stateChanged(int iconEnabled) // Icon theme checkbox logic
{
    if (ui->iconCheckBox->checkState() == 0) {
        ui->darkIcon->setEnabled(0);
        ui->lightIcon->setEnabled(0);
        ui->darkDropIcon->setEnabled(0);
        ui->lightDropIcon->setEnabled(0);
    } else {
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

void MainWindow::on_gtkCheckBox_stateChanged(int gtkEnabled) // GTK theme checkbox logic
{
    if (ui->gtkCheckBox->checkState() == 0) {
        ui->darkGtk->setEnabled(0);
        ui->lightGtk->setEnabled(0);
        ui->darkDropGtk->setEnabled(0);
        ui->lightDropGtk->setEnabled(0);
    } else {
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

void MainWindow::on_lightDropKvantumStyle_currentIndexChanged(
        const QString &lightKvantumStyleUN) //sets the kvantum style from the drop menu
{
    lightKvantumStyle = lightKvantumStyleUN;
}

void MainWindow::on_darkDropKvantumStyle_currentIndexChanged(const QString &darkKvantumStyleUN) {
    darkKvantumStyle = darkKvantumStyleUN;
}

void MainWindow::on_wallCheckBox_stateChanged(int wallEnabled) // Wallpaper checkbox logic
{
    if (ui->wallCheckBox->checkState() == 0) {
        ui->darkWall->setEnabled(0);
        ui->lightWall->setEnabled(0);
        ui->darkWallBtn->setEnabled(0);
        ui->lightWallBtn->setEnabled(0);
    } else {
        ui->darkWall->setEnabled(1);
        ui->lightWall->setEnabled(1);
        ui->darkWallBtn->setEnabled(1);
        ui->lightWallBtn->setEnabled(1);
    }
}

void MainWindow::on_lightWallBtn_clicked() // Set light wallpaper
{
    lightWall = QFileDialog::getOpenFileName(this, tr("Select Image"), QDir::homePath() + "/Pictures",
                                             tr("Images(*.png *.jpg *.jpeg *.bmp)"));
    QFileInfo lw(lightWall);
    QString lightWallName = lw.fileName();
    ui->lightWallBtn->setText(lightWallName);
    ui->lightWallBtn->setToolTip(lightWall);
}

void MainWindow::on_darkWallBtn_clicked() // Set dark wallpaper
{
    darkWall = QFileDialog::getOpenFileName(this, tr("Select Image"), QDir::homePath() + "/Pictures",
                                            tr("Images(*.png *.jpg *.jpeg *.bmp)"));
    QFileInfo dw(darkWall);
    QString darkWallName = dw.fileName();
    ui->darkWallBtn->setText(darkWallName);
    ui->darkWallBtn->setToolTip(darkWall);
}

void MainWindow::on_lightScriptBtn_clicked()
{
	lightScript = QFileDialog::getOpenFileName(this,
        tr("Run Script"), "/home", tr("Script Files(.sh) (*.sh)"));
}

void MainWindow::on_darkScriptBtn_clicked()
{
	darkScript = QFileDialog::getOpenFileName(this,
        tr("Run Script"), "/home", tr("Script Files(.sh) (*.sh)"));
}
void MainWindow::on_autoCheckBox_stateChanged(int automaticEnabled) // Logic for enabling scheduling of themes
{
    if (ui->autoCheckBox->checkState() == 0) {
        ui->scheduleRadioBtn->setEnabled(0);
        //ui->sunRadioBtn->setEnabled(0);
        ui->lightTimeLabel->setEnabled(0);
        ui->darkTimeLabel->setEnabled(0);
        ui->lightTimeEdit->setEnabled(0);
        ui->darkTimeEdit->setEnabled(0);
        utils.settings->setValue("schedule", false);
        utils.settings->sync();
        ui->resMsg->setText(tr("To disable automatic mode , Koi must be restarted."));
        ui->resMsg->setMessageType(KMessageWidget::Warning);
        ui->resMsg->animatedShow();
    } else {

        ui->scheduleRadioBtn->setEnabled(1);
        //ui->sunRadioBtn->setEnabled(1);
        ui->lightTimeLabel->setEnabled(1);
        ui->darkTimeLabel->setEnabled(1);
        ui->lightTimeEdit->setEnabled(1);
        ui->darkTimeEdit->setEnabled(1);
        if (ui->scheduleRadioBtn->isChecked() == 0) {
            ui->lightTimeLabel->setEnabled(0);
            ui->darkTimeLabel->setEnabled(0);
            ui->lightTimeEdit->setEnabled(0);
            ui->darkTimeEdit->setEnabled(0);
        }
        utils.settings->setValue("schedule", true);
        utils.settings->sync();
        ui->resMsg->setText(tr("To enable automatic mode, Koi must be restarted."));
        ui->resMsg->setMessageType(KMessageWidget::Warning);
        ui->resMsg->animatedShow();
    }
}

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

void MainWindow::on_notifyCheckBox_stateChanged(int notifyEnabled) {
    if (ui->notifyCheckBox->checkState() == 0) {
        utils.settings->setValue("notify", false);
    } else {
        utils.settings->setValue("notify", true);
    }
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
    auto *about = new About(this);
    about->open();
}

void MainWindow::on_actionHide_triggered() // Hide to tray
{
    this->setVisible(0);
}

//void MainWindow::on_actionRestart_triggered() {
//    QProcess restartProcess;
//    restartProcess.setProgram(QApplication::applicationFilePath());
//    qint64 pid;
//    restartProcess.startDetached(&pid);
//    exit(12);
//}

//this is being used because of kubuntu 18.04 does not support the above

void MainWindow::on_actionRestart_triggered()
{
	QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
	exit(12);
}
