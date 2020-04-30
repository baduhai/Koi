#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSystemTrayIcon;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason);

private slots:
    void on_prefsBtn_clicked();

    void on_backBtn_clicked();

    void on_styleCheckBox_stateChanged(int arg1);

    void on_colorCheckBox_stateChanged(int arg1);

    void on_iconCheckBox_stateChanged(int arg1);

    void on_cursorCheckBox_stateChanged(int arg1);

    void on_gtkCheckBox_stateChanged(int arg1);

    void on_applyBtn_clicked();

    void on_cancelBtn_clicked();

    void on_autoCheckBox_stateChanged(int arg1);

    void on_scheduleRadioBtn_toggled(bool checked);

    void on_actionQuit_triggered();

    void on_actionPrefs_triggered();

    void on_wallCheckBox_stateChanged(int arg1);

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon * trayIcon;
    QMenu* trayIconMenu;
    QMenu* createMenu();
};
#endif // MAINWINDOW_H
