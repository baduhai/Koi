#include "gtk.h"

void Gtk::setTheme(QString theme) {
  bus = new QDBusConnection(QDBusConnection::sessionBus());
  interface = new QDBusInterface("org.kde.GtkConfig", "/GtkConfig",
                                 "org.kde.GtkConfig", *bus);
  interface->call("setGtk2Theme", theme);
  interface->call("setGtk3Theme", theme);
  interface->call("setGtkTheme", theme);
}
