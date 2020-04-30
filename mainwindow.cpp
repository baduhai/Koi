#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setIcon(QIcon(":/resources/icons/heart.png"));
    this->trayIcon->setVisible(true);
    auto trayMenu = this->createMenu();
    this->trayIcon->setContextMenu(trayMenu);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QMenu* MainWindow::createMenu()
{
  auto actionMenuQuit = new QAction("&Quit", this);
  connect(actionMenuQuit, &QAction::triggered, qApp, &QCoreApplication::quit);

  auto actionMenuPrefs = new QAction("&Preferences", this);

  auto actionMenuDark = new QAction("Switch to &Dark", this);

  auto actionMenuLight = new QAction("Switch to &Light", this);

  auto trayMenu = new QMenu(this);
  trayMenu->addAction(actionMenuLight);
  trayMenu->addAction(actionMenuDark);
  trayMenu->addAction(actionMenuPrefs);
  trayMenu->addAction(actionMenuQuit);

  return trayMenu;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason_)
{
  switch (reason_) {
  case QSystemTrayIcon::Trigger:
    this->trayIcon->showMessage("Hello", "You clicked me!");
    break;
  default:
    ;
  }
}

void MainWindow::on_prefsBtn_clicked()
{
    ui->mainStack->setCurrentIndex(1);
}

void MainWindow::on_backBtn_clicked()
{
    ui->mainStack->setCurrentIndex(0);
}

void MainWindow::on_styleCheckBox_stateChanged(int styleEnabled)
{
    if (ui->styleCheckBox->checkState() == Qt::Unchecked)
    {
        ui->darkStyle->setEnabled(0);
        ui->lightStyle->setEnabled(0);
        ui->darkDropStyle->setEnabled(0);
        ui->lightDropStyle->setEnabled(0);
        ui->colorCheckBox->setEnabled(1);
        ui->iconCheckBox->setEnabled(1);
    }
    else
    {
        ui->darkStyle->setEnabled(1);
        ui->lightStyle->setEnabled(1);
        ui->darkDropStyle->setEnabled(1);
        ui->lightDropStyle->setEnabled(1);
        ui->colorCheckBox->setChecked(0);
        ui->colorCheckBox->setEnabled(0);
        ui->iconCheckBox->setChecked(0);
        ui->iconCheckBox->setEnabled(0);
    }

}

void MainWindow::on_colorCheckBox_stateChanged(int colorEnabled)
{
    if (ui->colorCheckBox->checkState() == Qt::Unchecked)
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

void MainWindow::on_iconCheckBox_stateChanged(int iconEnabled)
{
    if (ui->iconCheckBox->checkState() == Qt::Unchecked)
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

void MainWindow::on_cursorCheckBox_stateChanged(int cursorEnabled)
{
    if (ui->cursorCheckBox->checkState() == Qt::Unchecked)
    {
        ui->darkCursor->setEnabled(0);
        ui->lightCursor->setEnabled(0);
        ui->darkDropCursor->setEnabled(0);
        ui->lightDropCursor->setEnabled(0);
    }
    else
    {
        ui->darkCursor->setEnabled(1);
        ui->lightCursor->setEnabled(1);
        ui->darkDropCursor->setEnabled(1);
        ui->lightDropCursor->setEnabled(1);
    }
}

void MainWindow::on_gtkCheckBox_stateChanged(int gtkEnlabled)
{
    if (ui->gtkCheckBox->checkState() == Qt::Unchecked)
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

void MainWindow::on_wallCheckBox_stateChanged(int arg1)
{
    if (ui->wallCheckBox->checkState() == Qt::Unchecked)
    {
        ui->darkWall->setEnabled(0);
        ui->lightWall->setEnabled(0);
        ui->darkDropWall->setEnabled(0);
        ui->lightDropWall->setEnabled(0);
    }
    else
    {
        ui->darkWall->setEnabled(1);
        ui->lightWall->setEnabled(1);
        ui->darkDropWall->setEnabled(1);
        ui->lightDropWall->setEnabled(1);
    }
}

void MainWindow::on_applyBtn_clicked()
{
    ui->mainStack->setCurrentIndex(0);
}

void MainWindow::on_cancelBtn_clicked()
{
    ui->mainStack->setCurrentIndex(0);
}

void MainWindow::on_autoCheckBox_stateChanged(int automaticEnabled)
{
    if (ui->autoCheckBox->checkState() == Qt::Unchecked)
    {
        ui->scheduleRadioBtn->setEnabled(0);
        ui->sunRadioBtn->setEnabled(0);
        ui->lightTimeLabel->setEnabled(0);
        ui->darkTimeLabel->setEnabled(0);
        ui->timeEditLight->setEnabled(0);
        ui->timeEditDark->setEnabled(0);
    }
    else
    {
        ui->scheduleRadioBtn->setEnabled(1);
        ui->sunRadioBtn->setEnabled(1);
        ui->lightTimeLabel->setEnabled(1);
        ui->darkTimeLabel->setEnabled(1);
        ui->timeEditLight->setEnabled(1);
        ui->timeEditDark->setEnabled(1);
        if (ui->scheduleRadioBtn->isChecked() == Qt::Unchecked)
        {
            ui->lightTimeLabel->setEnabled(0);
            ui->darkTimeLabel->setEnabled(0);
            ui->timeEditLight->setEnabled(0);
            ui->timeEditDark->setEnabled(0);
        }
    }
}

void MainWindow::on_scheduleRadioBtn_toggled(bool scheduleSun)
{
    if (ui->sunRadioBtn->isChecked() == Qt::Unchecked)
    {
        ui->lightTimeLabel->setEnabled(1);
        ui->darkTimeLabel->setEnabled(1);
        ui->timeEditLight->setEnabled(1);
        ui->timeEditDark->setEnabled(1);
    }
    else
    {
        ui->lightTimeLabel->setEnabled(0);
        ui->darkTimeLabel->setEnabled(0);
        ui->timeEditLight->setEnabled(0);
        ui->timeEditDark->setEnabled(0);
    }

}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionPrefs_triggered()
{
    ui->mainStack->setCurrentIndex(1);
}

void MainWindow::on_actionAbout_triggered()
{

}
