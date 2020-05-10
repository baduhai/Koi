#ifndef GTK_H
#define GTK_H

#include <QString>
#include <QtDBus>

class Gtk
{
public:
    Gtk();

    void setGtk(QString gtkTheme);

private:
    QDBusConnection *bus;
    QDBusInterface *interface;
};

#endif // GTK_H
