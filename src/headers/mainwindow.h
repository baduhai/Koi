#pragma once

// Qt libraries
#include <QCloseEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QMessageBox>
#include <QSystemTrayIcon>

// Headers
#include "utils.h"
#include "trayManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr, Utils* utils = nullptr);
  void initTrayIcon();
  void initSettingsInterface();
  ~MainWindow();

private:
    bool isSettingLoaded;

    public slots:
  // Toggles the visibility of the settings
  void toggleVisibility();

  private slots:
  void closeEvent(QCloseEvent *event);


  void refreshDirs();
  void loadPrefs();
  void savePrefs();
  int prefsSaved();


  void on_prefsBtn_clicked();
  void on_backBtn_clicked();
  void on_applyBtn_clicked();
  void on_lightBtn_clicked();
  void on_darkBtn_clicked();
  // void on_refreshBtn_clicked();

  void on_styleCheckBox_stateChanged(int arg1);
  void on_colorCheckBox_stateChanged(int arg1);
  void on_iconCheckBox_stateChanged(int arg1);
  void on_gtkCheckBox_stateChanged(int arg1);
  void on_kvantumStyleCheckBox_stateChanged(int arg1);
  void on_wallCheckBox_stateChanged(int arg1);
  void on_scriptCheckBox_stateChanged(int arg1);
  void on_autoCheckBox_stateChanged(int arg1);
  void on_scheduleRadioBtn_toggled(bool checked);

  void on_actionQuit_triggered();
  void on_actionPrefs_triggered();
  void on_actionAbout_triggered();
  void on_actionHide_triggered();
  // void on_actionRefresh_triggered();

  void on_lightWallBtn_clicked();
  void on_darkWallBtn_clicked();
  void on_lightScriptBtn_clicked();
  void on_darkScriptBtn_clicked();
  void on_lightDropStyle_currentTextChanged(const QString &arg1);
  void on_darkDropStyle_currentTextChanged(const QString &arg1);
  void on_lightDropColor_currentTextChanged(const QString &arg1);
  void on_darkDropColor_currentTextChanged(const QString &arg1);
  void on_lightDropGtk_currentTextChanged(const QString &arg1);
  void on_darkDropGtk_currentTextChanged(const QString &arg1);
  void on_lightDropIcon_currentTextChanged(const QString &arg1);
  void on_darkDropIcon_currentTextChanged(const QString &arg1);
  void on_lightDropKvantumStyle_currentTextChanged(const QString &arg1);
  void on_darkDropKvantumStyle_currentTextChanged(const QString &arg1);
  void on_lightTimeEdit_userTimeChanged(const QTime &time);
  void on_darkTimeEdit_userTimeChanged(const QTime &time);
  void on_latitudeDSB_valueChanged(double lat);
  void on_longitudeDSB_valueChanged(double lon);
  void on_hiddenCheckBox_stateChanged(int arg1);
  void on_notifyCheckBox_stateChanged(int arg1);

  void on_actionRestart_triggered();

private:
  Ui::MainWindow *ui;
  TrayManager *trayManager;
  QMenu *trayMenu;
  QMenu *createMenu();
  QString scheduleType;
  QString lightStyle;
  QString darkStyle;
  QString lightColor;
  QString darkColor;
  QString lightIcon;
  QString darkIcon;
  QString lightGtk;
  QString darkGtk;
  QString lightWall;
  QString darkWall;
  QString lightScript;
  QString darkScript;
  QString lightKvantumStyle;
  QString darkKvantumStyle;
  QString lightTime;
  QString darkTime;
  Utils *utils;

protected:
};
