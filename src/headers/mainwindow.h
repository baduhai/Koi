#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// Headers
#include "about.h"
#include "utils.h"
//Qt libs
#include <QFileDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private slots:
    void refreshDirs();

    void loadPrefs();

    void savePrefs();

    void toggleVisibility();

    void iconActivated(QSystemTrayIcon::ActivationReason);

    void on_prefsBtn_clicked();

    void on_backBtn_clicked();

    void on_applyBtn_clicked();

    void on_styleCheckBox_stateChanged(int arg1);

    void on_colorCheckBox_stateChanged(int arg1);

    void on_iconCheckBox_stateChanged(int arg1);

    void on_gtkCheckBox_stateChanged(int arg1);

    void on_wallCheckBox_stateChanged(int arg1);

    void on_autoCheckBox_stateChanged(int arg1);

    void on_scheduleRadioBtn_toggled(bool checked);

    void on_actionQuit_triggered();

    void on_actionPrefs_triggered();

    void on_actionAbout_triggered();

    void on_actionHide_triggered();

    void on_refreshBtn_clicked();

    void on_lightWallBtn_clicked();

    void on_darkWallBtn_clicked();

    void on_lightDropStyle_currentIndexChanged(const QString &arg1);

    void on_darkDropStyle_currentIndexChanged(const QString &arg1);

    void on_lightDropColor_currentIndexChanged(const QString &arg1);

    void on_darkDropColor_currentIndexChanged(const QString &arg1);

    void on_lightDropGtk_currentIndexChanged(const QString &arg1);

    void on_darkDropGtk_currentIndexChanged(const QString &arg1);

    void on_lightDropIcon_currentIndexChanged(const QString &arg1);

    void on_darkDropIcon_currentIndexChanged(const QString &arg1);

    void on_actionRefresh_triggered();

    void on_lightTimeEdit_userTimeChanged(const QTime &time);

    void on_darkTimeEdit_userTimeChanged(const QTime &time);

    void on_lightBtn_clicked();

    void on_darkBtn_clicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon* trayIcon;
    QMenu* trayMenu;
    QMenu* createMenu();
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
    QString lightTime;
    QString darkTime;
    Utils utils;

protected:

};
#endif // MAINWINDOW_H
