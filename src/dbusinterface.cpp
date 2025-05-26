#include <QObject>
#include "headers/utils.h"
#include "koiadaptor.h"

KoiDbusInterface::KoiDbusInterface(QObject* parent,Utils* u) : QObject(parent) {
    new KoiDbusInterfaceAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Koi", this);
    dbus.registerService("dev.baduhai.Koi");
    utils = u;
    }

KoiDbusInterface::~KoiDbusInterface() {
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.unregisterService("dev.baduhai.Koi");
    dbus.unregisterObject("/Koi");
}


void KoiDbusInterface::goLightMode() {
    utils->goLight();
}

void KoiDbusInterface::goDarkMode() {
    utils->goDark();
}

void KoiDbusInterface::toggleMode() {
    utils->toggle();
}