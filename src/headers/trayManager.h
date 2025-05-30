// traymanager.h
#pragma once

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include "utils.h"

class MainWindow;

class TrayManager : public QObject {
  Q_OBJECT

public:
  explicit TrayManager(MainWindow *mainWindow, Utils *utils, QObject *parent = nullptr);
  void showTrayIcon();

private:
  QSystemTrayIcon *trayIcon;
  QMenu *trayMenu;
  MainWindow *mainWindow;
  Utils *utils;

  QMenu* createMenu();

private slots:
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
};