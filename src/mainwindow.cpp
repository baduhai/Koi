#include "headers/mainwindow.h"
#include "headers/trayManager.h"
#include "headers/about.h"
#include "ui/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Utils* utils)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
  this->isSettingLoaded = false;
  this->utils = utils;
  trayManager = new TrayManager(this, utils);
  trayManager->showTrayIcon();
}
void MainWindow::initSettingsInterface()
{
  this->utils->initialiseSettings();
  ui->setupUi(this);
  ui->mainStack->setCurrentIndex(0);
  refreshDirs();
  loadPrefs();
  ui->resMsg->hide();
  auto actionRes = new QAction("Restart", this);
  actionRes->setIcon(QIcon::fromTheme("view-refresh"));
  connect(actionRes, &QAction::triggered, this, &MainWindow::on_actionRestart_triggered);
  ui->resMsg->addAction(actionRes);
  this->isSettingLoaded = true;
}
MainWindow::~MainWindow() {
    this->setVisible(0);
    delete ui;
}

// Override window managing events
void MainWindow::closeEvent(QCloseEvent *event) { // Overide close event
  event->ignore();
  toggleVisibility();
}

// Independent functions
void MainWindow::loadPrefs() {
  // Load notify prefs
  if (utils->settings->value("notify", true).toBool()) {
    ui->notifyCheckBox->setChecked(true);
  } else {
    ui->notifyCheckBox->setChecked(false);
  }
  // Load startup prefs
  if (utils->settings->value("start-hidden").toBool()) {
    ui->hiddenCheckBox->setChecked(true);
  } else {
    ui->hiddenCheckBox->setChecked(false);
  }

  // Load scheduling prefs
  if (utils->settings->value("schedule").toBool()) {
    ui->autoCheckBox->setChecked(1);
  } else {
    ui->autoCheckBox->setChecked(0);
  }
  if (utils->settings->value("schedule-type") == "time") {
    ui->scheduleRadioBtn->setChecked(1);
    ui->lightTimeLabel->setVisible(true);
    ui->darkTimeLabel->setVisible(true);
    ui->lightTimeEdit->setVisible(true);
    ui->darkTimeEdit->setVisible(true);
    ui->latitudeLabel->setVisible(false);
    ui->longitudeLabel->setVisible(false);
    ui->latitudeDSB->setVisible(false);
    ui->longitudeDSB->setVisible(false);
  } else {
    ui->sunRadioBtn->setChecked(1);
    ui->lightTimeLabel->setVisible(false);
    ui->darkTimeLabel->setVisible(false);
    ui->lightTimeEdit->setVisible(false);
    ui->darkTimeEdit->setVisible(false);
    ui->latitudeLabel->setVisible(true);
    ui->longitudeLabel->setVisible(true);
    ui->latitudeDSB->setVisible(true);
    ui->longitudeDSB->setVisible(true);
  }

  ui->lightTimeEdit->setTime(utils->settings->value("time-light").toTime());
  ui->darkTimeEdit->setTime(utils->settings->value("time-dark").toTime());

  ui->latitudeDSB->setValue(utils->settings->value("latitude").toDouble());
  ui->longitudeDSB->setValue(utils->settings->value("longitude").toDouble());

  // Load Plasma style prefs
  if (utils->settings->value("PlasmaStyle/enabled").toBool()) {
    ui->styleCheckBox->setChecked(true);
  } else {
    ui->styleCheckBox->setChecked(false);
  }
  ui->lightDropStyle->setCurrentText(
      utils->settings->value("PlasmaStyle/light").toString());
  ui->darkDropStyle->setCurrentText(
      utils->settings->value("PlasmaStyle/dark").toString());

  // Load color scheme prefs
  if (utils->settings->value("ColorScheme/enabled").toBool()) {
    ui->colorCheckBox->setChecked(true);
  } else {
    ui->colorCheckBox->setChecked(false);
  }
  QFileInfo lightColorsPref(
      utils->settings->value("ColorScheme/light").toString());
  QFileInfo darkColorsPref(
      utils->settings->value("ColorScheme/dark").toString());
  QString lightColorsPrefString = lightColorsPref.baseName();
  QString darkColorsPrefString = darkColorsPref.baseName();
  ui->lightDropColor->setCurrentText(lightColorsPrefString);
  ui->darkDropColor->setCurrentText(darkColorsPrefString);

  // Load icon theme prefs
  if (utils->settings->value("IconTheme/enabled").toBool()) {
    ui->iconCheckBox->setChecked(true);
  } else {
    ui->iconCheckBox->setChecked(false);
  }
  ui->lightDropIcon->setCurrentText(
      utils->settings->value("IconTheme/light").toString());
  ui->darkDropIcon->setCurrentText(
      utils->settings->value("IconTheme/dark").toString());

  // Load GTK Theme prefs
  if (utils->settings->value("GTKTheme/enabled").toBool()) {
    ui->gtkCheckBox->setChecked(true);
  } else {
    ui->gtkCheckBox->setChecked(false);
  }
  ui->lightDropGtk->setCurrentText(
      utils->settings->value("GTKTheme/light").toString());
  ui->darkDropGtk->setCurrentText(
      utils->settings->value("GTKTheme/dark").toString());

  // Load Kvantum Style theme prefs
  if (utils->settings->value("KvantumStyle/enabled").toBool()) {
    ui->kvantumStyleCheckBox->setChecked(true);
  } else {
    ui->kvantumStyleCheckBox->setChecked(false);
  }
  // sets the displayed text on the combo box of the kvantum style.
  ui->lightDropKvantumStyle->setCurrentText(
      utils->settings->value("KvantumStyle/light").toString());
  ui->darkDropKvantumStyle->setCurrentText(
      utils->settings->value("KvantumStyle/dark").toString());

  // Load Wallpaper prefs
  if (utils->settings->value("Wallpaper/enabled").toBool()) {
    ui->wallCheckBox->setChecked(true);
  } else {
    ui->wallCheckBox->setChecked(false);
  }
  QFileInfo lw(utils->settings->value("Wallpaper/light").toString());
  QString lightWallBtnText = lw.fileName();
  if (lightWall.isEmpty()) {
    ui->lightWallBtn->setText("Select...");
  } else {
    ui->lightWallBtn->setText(lightWallBtnText);
  }
  QFileInfo dw(utils->settings->value("Wallpaper/dark").toString());
  QString darkWallBtnText = dw.fileName();
  if (darkWall.isEmpty()) {
    ui->darkWallBtn->setText("Select...");
  } else {
    ui->darkWallBtn->setText(darkWallBtnText);
  }

  // Load Script prefs
  if (utils->settings->value("Script/enabled").toBool()) {
    ui->scriptCheckBox->setChecked(true);
  } else {
    ui->scriptCheckBox->setChecked(false);
  }
  QFileInfo ls(utils->settings->value("Script/light").toString());
  QString lightScriptBtnText = ls.fileName();
  if (lightScript.isEmpty()) {
    ui->lightScriptBtn->setText("Select...");
  } else {
    ui->lightScriptBtn->setText(lightScriptBtnText);
  }
  QFileInfo ds(utils->settings->value("Script/dark").toString());
  QString darkScriptBtnText = ds.fileName();
  if (darkScript.isEmpty()) {
    ui->darkScriptBtn->setText("Select...");
  } else {
    ui->darkScriptBtn->setText(darkScriptBtnText);
  }
}

void MainWindow::savePrefs() {
  // Plasma Style enabling
  if (ui->styleCheckBox->isChecked() == 0) {
    utils->settings->setValue("PlasmaStyle/enabled", false);
  } else {
    utils->settings->setValue("PlasmaStyle/enabled", true);
  }
  // Plasma Style saving prefs
  utils->settings->setValue("PlasmaStyle/light", lightStyle);
  utils->settings->setValue("PlasmaStyle/dark", darkStyle);

  // Color scheme enabling
  if (ui->colorCheckBox->isChecked() == 0) {
    utils->settings->setValue("ColorScheme/enabled", false);
  } else {
    utils->settings->setValue("ColorScheme/enabled", true);
  }
  // Color scheme saving prefs
  utils->settings->setValue("ColorScheme/light", lightColor);
  utils->settings->setValue("ColorScheme/dark", darkColor);

  // Icon theme enabling
  if (ui->iconCheckBox->checkState() == 0) {
    utils->settings->setValue("IconTheme/enabled", false);
  } else {
    utils->settings->setValue("IconTheme/enabled", true);
  }
  // Icon theme saving prefs
  utils->settings->setValue("IconTheme/light", lightIcon);
  utils->settings->setValue("IconTheme/dark", darkIcon);

  // GTK theme enabling
  if (ui->gtkCheckBox->isChecked() == 0) {
    utils->settings->setValue("GTKTheme/enabled", false);
  } else {
    utils->settings->setValue("GTKTheme/enabled", true);
  }
  // GTK theme saving prefs
  utils->settings->setValue("GTKTheme/light", lightGtk);
  utils->settings->setValue("GTKTheme/dark", darkGtk);

  // Kvantum Style enabling
  if (ui->kvantumStyleCheckBox->isChecked() == 0) {
    utils->settings->setValue("KvantumStyle/enabled", false);
  } else {
    utils->settings->setValue("KvantumStyle/enabled", true);
  }
  // Kvantum Style Theme saving Prefs
  utils->settings->setValue("KvantumStyle/light", lightKvantumStyle);
  utils->settings->setValue("KvantumStyle/dark", darkKvantumStyle);

  // Wallpaper enabling
  if (ui->wallCheckBox->isChecked() == 0) {
    utils->settings->setValue("Wallpaper/enabled", false);
  } else {
    utils->settings->setValue("Wallpaper/enabled", true);
  }
  // Wallpaper saving prefs
  utils->settings->setValue("Wallpaper/light", lightWall);
  utils->settings->setValue("Wallpaper/dark", darkWall);
  utils->settings->sync();

  // Script enabling
  if (ui->scriptCheckBox->isChecked() == 0) {
    utils->settings->setValue("Script/enabled", false);
  } else {
    utils->settings->setValue("Script/enabled", true);
  }
  // Script saving prefs
  utils->settings->setValue("Script/light", lightScript);
  utils->settings->setValue("Script/dark", darkScript);
  utils->settings->sync();
}
void MainWindow::refreshDirs() // Refresh function to find new themes
{
  // Refresh plasma styles
  QStringList plasmaStyles = utils->getPlasmaStyles();
  ui->lightDropStyle->clear();
  ui->lightDropStyle->addItems(plasmaStyles);
  ui->darkDropStyle->clear();
  ui->darkDropStyle->addItems(plasmaStyles);
  // Refresh color schemes
  QStringList colorSchemes = utils->getColorSchemes();
  ui->lightDropColor->clear();
  ui->lightDropColor->addItems(colorSchemes);
  ui->darkDropColor->clear();
  ui->darkDropColor->addItems(colorSchemes);
  // Refresh icon themes
  QStringList iconThemes = utils->getIconThemes();
  ui->lightDropIcon->clear();
  ui->lightDropIcon->addItems(iconThemes);
  ui->darkDropIcon->clear();
  ui->darkDropIcon->addItems(iconThemes);
  // Refresh gtk themes
  QStringList gtkThemes = utils->getGtkThemes();
  ui->lightDropGtk->clear();
  ui->lightDropGtk->addItems(gtkThemes);
  ui->darkDropGtk->clear();
  ui->darkDropGtk->addItems(gtkThemes);
  // Refresh Kvantum Style themes.
  QStringList kvantumStyle = utils->getKvantumStyles();
  ui->lightDropKvantumStyle
      ->clear(); // clears everything from the kvantum drop down menu
  ui->darkDropKvantumStyle->clear();
  ui->lightDropKvantumStyle->addItems(
      kvantumStyle); // adds the new loaded kvantum styles
  ui->darkDropKvantumStyle->addItems(kvantumStyle);
  loadPrefs();
}
void MainWindow::toggleVisibility() {
  if(this->isSettingLoaded == false) {
    initSettingsInterface();
  }

  if (this->isVisible() == 0) {
    this->setVisible(1);
    this->activateWindow();
  } else {
    this->setVisible(0);
  }
}
int MainWindow::prefsSaved() // Lots of ifs, don't know how to do it any other
                             // way. Maybe an array?
{
  if (ui->styleCheckBox->isChecked() !=
      utils->settings->value("PlasmaStyle/enabled").toBool()) {
    return 0;
  }
  if (lightStyle != utils->settings->value("PlasmaStyle/light").toString()) {
    return 0;
  }
  if (darkStyle != utils->settings->value("PlasmaStyle/dark").toString()) {
    return 0;
  }
  if (ui->colorCheckBox->isChecked() !=
      utils->settings->value("ColorScheme/enabled").toBool()) {
    return 0;
  }
  if (lightColor != utils->settings->value("ColorScheme/light").toString()) {
    return 0;
  }
  if (darkColor != utils->settings->value("ColorScheme/dark").toString()) {
    return 0;
  }
  if (ui->iconCheckBox->isChecked() !=
      utils->settings->value("IconTheme/enabled").toBool()) {
    return 0;
  }
  if (lightIcon != utils->settings->value("IconTheme/light").toString()) {
    return 0;
  }
  if (darkIcon != utils->settings->value("IconTheme/dark").toString()) {
    return 0;
  }
  if (ui->gtkCheckBox->isChecked() !=
      utils->settings->value("GTKTheme/enabled").toBool()) {
    return 0;
  }
  if (lightGtk != utils->settings->value("GTKTheme/light").toString()) {
    return 0;
  }
  if (darkGtk != utils->settings->value("GTKTheme/dark").toString()) {
    return 0;
  }
  /*
  for kvantum style to make sure the settings is applied upon exit
  will try to change it to nested if statement as the drop down box is
  dependent on whether the check box is clicked
  */
  if (ui->kvantumStyleCheckBox->isChecked() !=
      utils->settings->value("KvantumStyle/enabled").toBool()) {
    return 0;
  }
  if (lightKvantumStyle !=
      utils->settings->value("KvantumStyle/light").toString()) {
    return 0;
  }
  if (darkKvantumStyle !=
      utils->settings->value("KvantumStyle/dark").toString()) {
    return 0;
  }
  if (ui->wallCheckBox->isChecked() !=
      utils->settings->value("Wallpaper/enabled").toBool()) {
    return 0;
  }
  if (lightWall != utils->settings->value("Wallpaper/light").toString()) {
    return 0;
  }
  if (darkWall != utils->settings->value("Wallpaper/dark").toString()) {
    return 0;
  }
  if (ui->scriptCheckBox->isChecked() !=
      utils->settings->value("Script/enabled").toBool()) {
    return 0;
  }
  if (lightScript != utils->settings->value("Script/light").toString()) {
    return 0;
  }
  if (darkScript != utils->settings->value("Script/dark").toString()) {
    return 0;
  }
  return 1;
}


// Functionality of buttons - Related to program navigation, interaction and
// saving settings
void MainWindow::on_prefsBtn_clicked() // Preferences button - Sets all
                                       // preferences as found in koirc file
{
  lightWall = utils->settings->value("Wallpaper/light").toString();
  darkWall = utils->settings->value("Wallpaper/dark").toString();
  /*
   * The two lines above fix a bug where when applying the settings without
   * having changed the wallpapers, the wallpaper preferences would be set as
   * empty strings, and not stay the same, as is expected behaviour. Unsure why
   * this fixes said bug... ¯\_(ツ)_/¯
   */
  lightScript = utils->settings->value("Script/light").toString();
  darkScript = utils->settings->value("Script/dark").toString();
  // Added the above lines in just in case the same bug affects setting the
  // script properly
  loadPrefs();
  ui->mainStack->setCurrentIndex(1);
}
void MainWindow::on_backBtn_clicked() // Back button in preferences view - Must
                                      // setup cheking if prefs saved
{
  if (prefsSaved()) {
    ui->mainStack->setCurrentIndex(0);
  } else {
    QMessageBox applyConfs; // Verify if user wants to save settings
    applyConfs.setWindowTitle("Save Settings — Koi");
    applyConfs.setText(
        "You have unsaved changes, would you like to save or discard them?");
    applyConfs.setIcon(QMessageBox::Warning);
    applyConfs.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                                  QMessageBox::Cancel);
    applyConfs.setDefaultButton(QMessageBox::Save);
    int ret = applyConfs.exec();
    switch (ret) {
    case QMessageBox::Save: // Save and change stack
      savePrefs();
      ui->mainStack->setCurrentIndex(0);
      break;
    case QMessageBox::Discard: // Change stack
      ui->mainStack->setCurrentIndex(0);
      lightWall = utils->settings->value("Wallpaper/light").toString();
      darkWall = utils->settings->value("Wallpaper/dark").toString();
      lightScript = utils->settings->value("Script/light").toString();
      darkScript = utils->settings->value("Script/dark").toString();
      loadPrefs();
      break;
    case QMessageBox::Cancel: // Do nothin  //probably dont need this case
      break;
    default:
      break;
    }
  }
}
void MainWindow::on_applyBtn_clicked() { savePrefs(); }
void MainWindow::on_lightBtn_clicked() { utils->goLight(); }
void MainWindow::on_darkBtn_clicked() { utils->goDark(); }
/*void MainWindow::on_refreshBtn_clicked() // Refresh dirs contents
{
    loadPrefs();
    refreshDirs();
}*/

// Editing options
void MainWindow::on_styleCheckBox_stateChanged(
    int styleEnabled) // Plasma style checkbox logic
{
  ui->darkStyle->setEnabled(styleEnabled);
  ui->lightStyle->setEnabled(styleEnabled);
  ui->darkDropStyle->setEnabled(styleEnabled);
  ui->lightDropStyle->setEnabled(styleEnabled);
}
void MainWindow::on_colorCheckBox_stateChanged(
    int colorEnabled) // Color scheme checkbox logic
{
  ui->darkColor->setEnabled(colorEnabled);
  ui->lightColor->setEnabled(colorEnabled);
  ui->darkDropColor->setEnabled(colorEnabled);
  ui->lightDropColor->setEnabled(colorEnabled);
}
void MainWindow::on_iconCheckBox_stateChanged(
    int iconEnabled) // Icon theme checkbox logic
{
  ui->darkIcon->setEnabled(iconEnabled);
  ui->lightIcon->setEnabled(iconEnabled);
  ui->darkDropIcon->setEnabled(iconEnabled);
  ui->lightDropIcon->setEnabled(iconEnabled);
}
void MainWindow::on_kvantumStyleCheckBox_stateChanged(
    int kvantumStyleEnabled) // kvantum theme checkbox logic used to perform an
                             // actioni after enabling the check box
{
  ui->darkKvantumStyle->setEnabled(kvantumStyleEnabled);
  ui->lightDropKvantumStyle->setEnabled(kvantumStyleEnabled);
  ui->darkDropKvantumStyle->setEnabled(kvantumStyleEnabled);
  ui->darkKvantumStyle->setEnabled(kvantumStyleEnabled);
}
void MainWindow::on_lightDropStyle_currentTextChanged(
    const QString &lightStyleUN) // Set light plasma style
{
  lightStyle = lightStyleUN;
}
void MainWindow::on_darkDropStyle_currentTextChanged(
    const QString &darkStyleUN) // Set dark plasma style
{
  darkStyle = darkStyleUN;
}
void MainWindow::on_lightDropColor_currentTextChanged(
    const QString &lightColorUN) // Set light color scheme
{
  lightColor = lightColorUN;
}
void MainWindow::on_darkDropColor_currentTextChanged(
    const QString &darkColorUN) // Set dark color scheme
{
  darkColor = darkColorUN;
}
void MainWindow::on_lightDropIcon_currentTextChanged(
    const QString &lightIconUN) // Set light icon theme
{
  lightIcon = lightIconUN;
}
void MainWindow::on_darkDropIcon_currentTextChanged(
    const QString &darkIconUN) // Set dark icon theme
{
  darkIcon = darkIconUN;
}
void MainWindow::on_gtkCheckBox_stateChanged(
    int gtkEnabled) // GTK theme checkbox logic
{
  ui->darkGtk->setEnabled(gtkEnabled);
  ui->lightGtk->setEnabled(gtkEnabled);
  ui->darkDropGtk->setEnabled(gtkEnabled);
  ui->lightDropGtk->setEnabled(gtkEnabled);
}
void MainWindow::on_lightDropGtk_currentTextChanged(
    const QString &lightGtkUN) // Set light gtk theme
{
  lightGtk = lightGtkUN;
}
void MainWindow::on_darkDropGtk_currentTextChanged(
    const QString &darkGtkUN) // Set dark gtk theme
{
  darkGtk = darkGtkUN;
}
void MainWindow::on_lightDropKvantumStyle_currentTextChanged(
    const QString
        &lightKvantumStyleUN) // sets the kvantum style from the drop menu
{
  lightKvantumStyle = lightKvantumStyleUN;
}
void MainWindow::on_darkDropKvantumStyle_currentTextChanged(
    const QString &darkKvantumStyleUN) {
  darkKvantumStyle = darkKvantumStyleUN;
}
void MainWindow::on_wallCheckBox_stateChanged(
    int wallEnabled) // Wallpaper checkbox logic
{
  ui->darkWall->setEnabled(wallEnabled);
  ui->lightWall->setEnabled(wallEnabled);
  ui->darkWallBtn->setEnabled(wallEnabled);
  ui->lightWallBtn->setEnabled(wallEnabled);
}
void MainWindow::on_lightWallBtn_clicked() // Set light wallpaper
{
  lightWall = QFileDialog::getOpenFileName(
      this, tr("Select Image"), QDir::homePath() + "/Pictures",
      tr("Images(*.png *.jpg *.jpeg *.bmp)"));
  QFileInfo lw(lightWall);
  QString lightWallName = lw.fileName();
  ui->lightWallBtn->setText(lightWallName);
  ui->lightWallBtn->setToolTip(lightWall);
}
void MainWindow::on_darkWallBtn_clicked() // Set dark wallpaper
{
  darkWall = QFileDialog::getOpenFileName(
      this, tr("Select Image"), QDir::homePath() + "/Pictures",
      tr("Images(*.png *.jpg *.jpeg *.bmp)"));
  QFileInfo dw(darkWall);
  QString darkWallName = dw.fileName();
  ui->darkWallBtn->setText(darkWallName);
  ui->darkWallBtn->setToolTip(darkWall);
}
void MainWindow::on_scriptCheckBox_stateChanged(
    int scriptEnabled) // Script checkbox logic
{
  ui->darkScript->setEnabled(scriptEnabled);
  ui->lightScript->setEnabled(scriptEnabled);
  ui->darkScriptBtn->setEnabled(scriptEnabled);
  ui->lightScriptBtn->setEnabled(scriptEnabled);
}
void MainWindow::on_lightScriptBtn_clicked() // Set light script
{
  lightScript =
      QFileDialog::getOpenFileName(this, tr("Select Script"), QDir::homePath(),
                                   tr("Bash script files(*.sh)"));
  QFileInfo lw(lightScript);
  QString lightScriptName = lw.fileName();
  ui->lightScriptBtn->setText(lightScriptName);
  ui->lightScriptBtn->setToolTip(lightScript);
}
void MainWindow::on_darkScriptBtn_clicked() // Set dark script
{
  darkScript =
      QFileDialog::getOpenFileName(this, tr("Select Script"), QDir::homePath(),
                                   tr("Bash script files(*.sh)"));
  QFileInfo dw(darkScript);
  QString darkScriptName = dw.fileName();
  ui->darkScriptBtn->setText(darkScriptName);
  ui->darkScriptBtn->setToolTip(darkScript);
}
void MainWindow::on_autoCheckBox_stateChanged(
    int automaticEnabled) // Logic for enabling scheduling of themes
{
  ui->scheduleRadioBtn->setEnabled(automaticEnabled);
  ui->sunRadioBtn->setEnabled(automaticEnabled);

  ui->lightTimeLabel->setEnabled(automaticEnabled);
  ui->darkTimeLabel->setEnabled(automaticEnabled);
  ui->lightTimeEdit->setEnabled(automaticEnabled);
  ui->darkTimeEdit->setEnabled(automaticEnabled);
  ui->latitudeLabel->setEnabled(automaticEnabled);
  ui->longitudeLabel->setEnabled(automaticEnabled);
  ui->latitudeDSB->setEnabled(automaticEnabled);
  ui->longitudeDSB->setEnabled(automaticEnabled);

  utils->settings->setValue("schedule", automaticEnabled);
  utils->settings->sync();
  if (automaticEnabled) {
    ui->resMsg->setText(tr("To enable automatic mode, Koi must be restarted."));
  } else {
    ui->resMsg->setText(
        tr("To disable automatic mode, Koi must be restarted."));
  }
  ui->resMsg->setMessageType(KMessageWidget::Warning);
  ui->resMsg->animatedShow();
}
void MainWindow::on_scheduleRadioBtn_toggled(
    bool scheduleSun) // Toggle between manual schedule, and sun schedule
{
  if (scheduleSun) {
    scheduleType = "time";

    ui->lightTimeLabel->setVisible(true);
    ui->darkTimeLabel->setVisible(true);
    ui->lightTimeEdit->setVisible(true);
    ui->darkTimeEdit->setVisible(true);
    ui->latitudeLabel->setVisible(false);
    ui->longitudeLabel->setVisible(false);
    ui->latitudeDSB->setVisible(false);
    ui->longitudeDSB->setVisible(false);

  } else {
    scheduleType = "sun";

    ui->lightTimeLabel->setVisible(false);
    ui->darkTimeLabel->setVisible(false);
    ui->lightTimeEdit->setVisible(false);
    ui->darkTimeEdit->setVisible(false);
    ui->latitudeLabel->setVisible(true);
    ui->longitudeLabel->setVisible(true);
    ui->latitudeDSB->setVisible(true);
    ui->longitudeDSB->setVisible(true);
  }
  utils->settings->setValue("schedule-type", scheduleType);
  utils->settings->sync();

  ui->resMsg->setText(tr("To change automatic mode, Koi must be restarted."));
  ui->resMsg->setMessageType(KMessageWidget::Warning);
  ui->resMsg->animatedShow();
}
void MainWindow::on_lightTimeEdit_userTimeChanged(
    const QTime &time) // Set light time
{
  lightTime = time.toString();
  utils->settings->setValue("time-light", lightTime);
  utils->settings->sync();
  ui->resMsg->setText(tr("Koi must be restarted for new schedule to be used."));
  ui->resMsg->setMessageType(KMessageWidget::Warning);
  ui->resMsg->animatedShow();
}
void MainWindow::on_darkTimeEdit_userTimeChanged(
    const QTime &time) // Set dark time
{
  darkTime = time.toString();
  utils->settings->setValue("time-dark", darkTime);
  utils->settings->sync();
  ui->resMsg->setText(tr("Koi must be restarted for new schedule to be used."));
  ui->resMsg->setMessageType(KMessageWidget::Warning);
  ui->resMsg->animatedShow();
}
void MainWindow::on_latitudeDSB_valueChanged(double lat) {
  utils->settings->setValue("latitude", lat);
  utils->settings->sync();
  ui->resMsg->setText(
      tr("Koi must be restarted for new coordinates to be used."));
  ui->resMsg->setMessageType(KMessageWidget::Warning);
  ui->resMsg->animatedShow();
}
void MainWindow::on_longitudeDSB_valueChanged(double lon) {
  utils->settings->setValue("longitude", lon);
  utils->settings->sync();
  ui->resMsg->setText(
      tr("Koi must be restarted for new coordinates to be used."));
  ui->resMsg->setMessageType(KMessageWidget::Warning);
  ui->resMsg->animatedShow();
}
void MainWindow::on_hiddenCheckBox_stateChanged(int hiddenEnabled) {
  utils->settings->setValue("start-hidden", hiddenEnabled);
}
void MainWindow::on_notifyCheckBox_stateChanged(int notifyEnabled) {
  utils->settings->setValue("notify", notifyEnabled);
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
/*void MainWindow::on_actionRefresh_triggered() // Refresh dirs
{
    on_refreshBtn_clicked();
}*/
void MainWindow::on_actionRestart_triggered() {
  QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
  exit(12);
}
