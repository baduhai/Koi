#include "gtk.h"
#include "../headers/utils.h"

void Gtk::setTheme(QString theme) {

  if (!interface) {
    interface = new QDBusInterface("org.kde.GtkConfig", "/GtkConfig",
                                   "org.kde.GtkConfig",
                                   QDBusConnection::sessionBus());
  }

  interface->call("setGtk2Theme", theme);
  interface->call("setGtk3Theme", theme);
  interface->call("setGtkTheme", theme);
}

QStringList Gtk::getThemes() {
  const QStringList searchPaths = Utils::systemDataSearchPaths(
    QStringLiteral("themes"),
    {QDir::homePath() + QStringLiteral("/.themes")});

  QStringList gtkThemes;
  for (const QString &dirPath : searchPaths) {
    const QDir dir(dirPath);
    gtkThemes.append(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
  }

  gtkThemes.removeDuplicates();
  gtkThemes.sort();
  return gtkThemes;
}
