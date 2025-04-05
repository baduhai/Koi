#pragma once

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
  QDBusInterface* interface = nullptr;
};
