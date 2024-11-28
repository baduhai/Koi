#pragma once

#include <QProcess>
#include <QString>
#include <QStringList>
#include <QtDBus>

// Generic plugin applicable to all supported plugins.
class Plugin {
public:
  virtual void setTheme(QString themeName) = 0;
  virtual QStringList getThemes() { return QStringList(); };
};

// Plugin that handles theme switching via DBus communication.
class DbusPlugin : Plugin {
protected:
  QDBusConnection *bus;
  QDBusInterface *interface;
};

// Plugin that handles theme switching via an external process.
class ProcessPlugin : Plugin {
protected:
  QProcess *process;
};