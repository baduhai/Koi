#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//Headers
#include "about.h"
#include "utils.h"
#include "settings/settingdialog.h"
//Qt libs
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCloseEvent>
//Other libs
#include "lib/Scheduler.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
Q_OBJECT
public:

    void showWindow();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    //Signal to hide setting from systemTray.
    void showSettingsTray(bool isHidden);

public slots:

private slots:
    //Hides the Main Window instead of closing/destroying it
    void closeEvent(QCloseEvent *event);
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void toggleVisibility();

    //Main Page
    void showSettingsDialog();
    void on_actionAbout_triggered();
    void on_actionRestart_triggered();
private:
    //schedules the profiles that are in favList
    void runSchedule();
    void schedule(Profile *, QTime time);

    void loadSystemTray();
    static void profileEnabled(const QString &name);

    void createMenu();

    /**most times the app is started hidden and there is no
     * need for a UI so setupUI os only called when the GUI needs to be shown
     */
    void setupUi();
    /*** the Ui is not created/setup when the class is
     * instantiated isSetup stores if the the UI is setup
     */
    bool isSetup;
    Ui::MainWindow *ui;
    QSystemTrayIcon *_trayIcon;
    QMenu *_trayMenu;
    QSettings m_settings;
    SettingDialog *_settingDialog;
    QList<Profile *> favList;
};
#endif // MAINWINDOW_H
