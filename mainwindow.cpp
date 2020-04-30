#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"

#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Show system tray on startup
    trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setIcon(QIcon(":/resources/icons/koi_tray.png")); // Set tray icon
    this->trayIcon->setVisible(true);
    auto trayMenu = this->createMenu();
    this->trayIcon->setContextMenu(trayMenu); // Set tray context menu
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    // Show window on startup
    ui->setupUi(this);
    ui->mainStack->setCurrentIndex(0); // Always start window on main view
}

MainWindow::~MainWindow()
{
    delete ui;
}

QMenu* MainWindow::createMenu() // Define context menu items - Right click to show context menu
{
    auto actionMenuQuit = new QAction("&Quit", this);
    connect(actionMenuQuit, &QAction::triggered, qApp, &QCoreApplication::quit);

    auto actionMenuDark = new QAction("&Dark", this);

    auto actionMenuLight = new QAction("&Light", this);

    auto trayMenu = new QMenu(this);
    trayMenu->addAction(actionMenuLight);
    trayMenu->addAction(actionMenuDark);
    trayMenu->addAction(actionMenuQuit);

    return trayMenu;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason_)
{
    switch (reason_)
    {
        case QSystemTrayIcon::Trigger: // Left-click to toggle window visibility
            if (this->isVisible() == 1)
            {
                this->setVisible(0);
            }
            else
            {
                this->setVisible(1);
            }
            break;
        case QSystemTrayIcon::MiddleClick: // Middle-click to toggle between light and dark
            this->trayIcon->showMessage("Hello", "You middle-clicked me!"); // Must implement toggle
            break;
        default:
            break;
    }
}

void MainWindow::on_prefsBtn_clicked() // Preferences button
{
    ui->mainStack->setCurrentIndex(1);
}

void MainWindow::on_backBtn_clicked() // Back button in preferences view
{
    ui->mainStack->setCurrentIndex(0);
}

void MainWindow::on_styleCheckBox_stateChanged(int styleEnabled) // Plasma style checkbox logic
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

void MainWindow::on_colorCheckBox_stateChanged(int colorEnabled) // Color scheme checkbox logic
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

void MainWindow::on_iconCheckBox_stateChanged(int iconEnabled) // Icon theme checkbox logic
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

void MainWindow::on_cursorCheckBox_stateChanged(int cursorEnabled) // Cursor theme checkbox logic
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

void MainWindow::on_gtkCheckBox_stateChanged(int gtkEnlabled) // GTK theme checkbox logic
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

void MainWindow::on_wallCheckBox_stateChanged(int wallEnabled) // Wallpaper checkbox logic
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

void MainWindow::on_applyBtn_clicked() // Apply button on preferences view
{
    // Must implement saving settings
    ui->mainStack->setCurrentIndex(0);
}

void MainWindow::on_cancelBtn_clicked() // Cancel button on preferences view
{
    // Must implements not saving settings
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
    auto* about = new About(this);
    about->open();
}

void MainWindow::on_actionHide_triggered()
{
    this->setVisible(0);
}
