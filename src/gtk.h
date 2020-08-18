#ifndef GTK_H
#define GTK_H

#include <QString>
#include <QtDBus>
#include <QFileInfo>

class Gtk
{
public:
    Gtk();

    void setGtk(QString gtkTheme);
    static QStringList getGtkThemes();
private:
    QDBusConnection *bus;
    QDBusInterface *interface;
};

#endif // GTK_H
