#include "headers/colorscheme.h"

ColorScheme::ColorScheme()
{

}

void ColorScheme::setColorScheme(QString colorScheme)
{
    // Setting color scheme in user kde config
    QString kdeGlobalsLocation = QStandardPaths::locate(QStandardPaths::GenericConfigLocation, "kdeglobals");
    KSharedConfigPtr schemeSource = KSharedConfig::openConfig(colorScheme, KSharedConfig::CascadeConfig);
    KSharedConfigPtr userDestiny = KSharedConfig::openConfig(kdeGlobalsLocation, KSharedConfig::CascadeConfig);
    for (const QString &group: schemeSource->groupList())
    {
        if (group == "General")
        {
            continue;
        }
        KConfigGroup schemeSourceCG(schemeSource, group);
        KConfigGroup userDestinyCG(userDestiny, group);
        schemeSourceCG.copyTo(&userDestinyCG);
    }
    KConfigGroup(userDestiny, "General").writeEntry("ColorScheme", QFileInfo(colorScheme).baseName());
    userDestiny->sync();
    // Notifying Plasma of changes to color scheme
    QDBusMessage notify = QDBusMessage::createSignal("/KGlobalSettings", "org.kde.KGlobalSettings", "notifyChange");
    notify.setArguments({0, 0});
    QDBusConnection::sessionBus().send(notify);
    // Notify Plasma to regenerate Breeze gtk with new colors
    breezeGtkProcess = new QProcess();
    breezeGtkProcess->start("krdb");
}
