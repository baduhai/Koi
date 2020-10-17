#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//Headers
#include "about.h"
#include "utils.h"
#include "settings/settingdialog.h"
//Qt libs
#include <QFileDialog>
#include <QFileInfo>
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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    //Signal to hide setting from systemTray.
    void hideSettingsTray(bool isHidden);

public slots:

private slots:
    //Hides the Main Window instead of closing/destroying it
    void closeEvent(QCloseEvent *event);

    void iconActivated(QSystemTrayIcon::ActivationReason);

    void toggleVisibility();

    //Main Page
    void on_prefsBtn_clicked();
    void on_lightBtn_clicked();
    void on_darkBtn_clicked();

    void on_actionQuit_triggered();
    void on_actionPrefs_triggered();

    void on_actionAbout_triggered();
    void on_actionHide_triggered();

    void on_actionRestart_triggered();
private:
    void runSchedule();
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QMenu *createMenu();
    QSettings m_settings;

    QList<Profile *> favList;

    void schedule(Profile *, QTime time);
    void loadSystemTray();
    void profileEnabled(const QString &name);
};
#endif // MAINWINDOW_H
