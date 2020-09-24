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

public slots:

private slots:
	void closeEvent(QCloseEvent *event);

	void iconActivated(QSystemTrayIcon::ActivationReason);

	void toggleVisibility();

	//Main Page
	void on_prefsBtn_clicked();
	void on_lightBtn_clicked();
	void on_darkBtn_clicked();

	//Settings page 1

	//Plasma Buttons

	//  void on_autoCheckBox_stateChanged(int arg1);


	//void on_scheduleRadioBtn_toggled(bool checked);
	//Global Menu Settings
	void on_actionQuit_triggered();
	void on_actionPrefs_triggered();

	void on_actionAbout_triggered();
	void on_actionHide_triggered();

	void on_hiddenCheckBox_stateChanged(int arg1);

	void on_actionRestart_triggered();
private:
	Ui::MainWindow *ui;
	QSystemTrayIcon *trayIcon;
	QMenu *trayMenu;
	QMenu *createMenu();
	QSettings *settings;
	QHash<QTime, Utils *> schedProfiles;

	void schedule( Utils *utils,QTime time);
};
#endif // MAINWINDOW_H
