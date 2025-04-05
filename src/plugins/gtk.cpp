#include "gtk.h"

void Gtk::setTheme(QString theme) {

  if(!interface)
  {
      interface = new QDBusInterface("org.kde.GtkConfig", "/GtkConfig",
                                         "org.kde.GtkConfig", QDBusConnection::sessionBus());
  }

  interface->call("setGtk2Theme", theme);
  interface->call("setGtk3Theme", theme);
  interface->call("setGtkTheme", theme);
}

QStringList Gtk::getThemes() {
  QDir gtkLocalDir(QDir::homePath() + "/.themes");
  QDir gtkSystemDir("/usr/share/themes");
  QDir gtkNixDir("/var/run/current-system/sw/share/themes");
  QStringList gtkThemes;
  if (gtkLocalDir.exists()) {
    gtkThemes =
        gtkThemes + gtkLocalDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ;
  }
  if (gtkSystemDir.exists()) {
    gtkThemes =
        gtkThemes + gtkSystemDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ;
  };
  if (gtkNixDir.exists()) {
    gtkThemes =
        gtkThemes + gtkNixDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ;
  }
  gtkThemes.removeDuplicates();
  gtkThemes.sort();
  return gtkThemes;
}
