#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//Headers
#include "about.h"
#include "utils.h"
#include "lnflogic.h"
#include "lnflistmodel.h"
//Qt libs
#include <QFileDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCloseEvent>
//Other libs
#include "Scheduler.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private slots:
    void closeEvent(QCloseEvent *event);

    void iconActivated(QSystemTrayIcon::ActivationReason);

    void refreshDirs();
    void loadPrefs();
    void savePrefs();
    void toggleVisibility();
    int prefsSaved();
    void scheduleLight();
    void scheduleDark();

    //Main Page

    void on_prefsBtn_clicked();
    void on_lightBtn_clicked();
    void on_darkBtn_clicked();

    //Settings page 1

    //Plasma Buttons
    void on_lightDropStyle_currentIndexChanged(const QString &arg1);
    void on_darkDropStyle_currentIndexChanged(const QString &arg1);
    void on_styleCheckBox_stateChanged(int arg1);

    //Color Scheme Buttons
    void on_colorCheckBox_stateChanged(int arg1);
    void on_lightDropColor_currentIndexChanged(const QString &arg1);
    void on_darkDropColor_currentIndexChanged(const QString &arg1);

    //Icon Scheme Buttons
    void on_lightDropIcon_currentIndexChanged(const QString &arg1);
    void on_darkDropIcon_currentIndexChanged(const QString &arg1);
    void on_iconCheckBox_stateChanged(int arg1);

    //Gtk theme Buttons

    void on_gtkCheckBox_stateChanged(int arg1);
    void on_lightDropGtk_currentIndexChanged(const QString &arg1);
    void on_darkDropGtk_currentIndexChanged(const QString &arg1);

    //Cursor theme Buttons
    void on_lightDropCursor_currentIndexChanged(const QString &lightCursorUN);
    void on_darkDropCursor_currentIndexChanged(const QString &LightCursorUN);

    void on_lightDropWidget_currentIndexChanged(const QString&lightWidgetUN);
    void on_darkDropWidget_currentIndexChanged(const QString &darkWidgetUN);
    //WallPaper Buttons
    void on_lightWallBtn_clicked();

    void on_darkWallBtn_clicked();

    void on_backBtn_clicked();
    //void on_actionRefresh_triggered();
    void on_applyBtn_clicked();
    void on_kvantumStyleCheckBox_stateChanged(int arg1);
    void on_advancedPageBtn_clicked();
    void on_lightDropKvantumStyle_currentIndexChanged(const QString &arg1);
    void on_darkDropKvantumStyle_currentIndexChanged(const QString &arg1);
    void on_lightTimeEdit_userTimeChanged(const QTime &time);
    void on_darkTimeEdit_userTimeChanged(const QTime &time);

    void on_wallCheckBox_stateChanged(int arg1);
    //settings page 2
    void on_advancedPageHomeBtn_clicked();

    void on_advancedPageBackBtn_clicked();
    void on_autoCheckBox_stateChanged(int arg1);


    void on_scheduleRadioBtn_toggled(bool checked);
    //Global Menu Settings
    void on_actionQuit_triggered();
    void on_actionPrefs_triggered();

    void on_actionAbout_triggered();
    void on_actionHide_triggered();

    void on_hiddenCheckBox_stateChanged(int arg1);

    void on_notifyCheckBox_stateChanged(int arg1);
    void on_actionRestart_triggered();
private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
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
    QString lightKvantumStyle;
    QString darkKvantumStyle;
    QString lightTime;
    QString lightCursor;
    QString darkCursor;
    QString darkTime;
    QString lightWidget;
    QString darkWidget;
    Utils utils;
protected:
};
#endif // MAINWINDOW_H
