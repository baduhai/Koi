// traymanager.cpp
#include "headers/trayManager.h"
#include "headers/mainwindow.h"
#include <QIcon>
#include <QCoreApplication>

TrayManager::TrayManager(MainWindow *mainWindow, Utils *utils, QObject *parent)
    : QObject(parent), mainWindow(mainWindow), utils(utils) {

  trayIcon = new QSystemTrayIcon(this);
  QIcon icon = QIcon::fromTheme("koi_tray", QIcon(":/resources/icons/koi_tray.png"));
  trayIcon->setIcon(icon);
  trayIcon->setContextMenu(createMenu());

  connect(trayIcon, &QSystemTrayIcon::activated, this, &TrayManager::iconActivated);
}

void TrayManager::showTrayIcon() {
  trayIcon->setVisible(true);
}

QMenu* TrayManager::createMenu() {
  auto *menu = new QMenu();

  auto toggle = new QAction("&Toggle Window", this);
  connect(toggle, &QAction::triggered, mainWindow, &MainWindow::toggleVisibility);

  auto light = new QAction("&Light", this);
  connect(light, &QAction::triggered, [this]() { utils->goLight(); });

  auto dark = new QAction("&Dark", this);
  connect(dark, &QAction::triggered, [this]() { utils->goDark(); });

  auto quit = new QAction("&Quit", this);
  connect(quit, &QAction::triggered, QCoreApplication::quit);

  menu->addAction(toggle);
  menu->addAction(light);
  menu->addAction(dark);
  menu->addAction(quit);

  return menu;
}

void TrayManager::iconActivated(QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
    case QSystemTrayIcon::Trigger:
      utils->toggle();
      break;
    case QSystemTrayIcon::MiddleClick:
      utils->toggle();
      break;
    default:
      break;
  }
}
