#include "colorscheme.h"

ColorScheme::ColorScheme()
{

}
//TODO delete these as it is no longer need anymore
void ColorScheme::setColorScheme(const QString &colorScheme)
{
    // Notifying Plasma of changes to color scheme
    //TODO recheck if this is actually needed
    QDBusMessage notify = QDBusMessage::createSignal("/KGlobalSettings", "org.kde.KGlobalSettings", "notifyChange");
    notify.setArguments({0, 0});
    QDBusConnection::sessionBus().send(notify);
    // Notify Plasma to regenerate Breeze gtk with new colors
    breezeGtkProcess = new QProcess();
    breezeGtkProcess->setProgram(QStringLiteral("krdb"));
    breezeGtkProcess->start();
    breezeGtkProcess->waitForFinished();
}
QStringList ColorScheme::getColorSchemes() // Get all available color schemes
{
    QDir colorsLocalDir(QDir::homePath() + "/.local/share/color-schemes");
    colorsLocalDir.setNameFilters(QStringList() << "*.colors");
    colorsLocalDir.setFilter(QDir::Files);
    colorsLocalDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesLocal = colorsLocalDir.entryInfoList();
    QStringList colorSchemesLocalNames;
    //TODO remember to ask about qasconst in qt reddit group
    for (const auto &color : qAsConst(colorSchemesLocal))
    {
        colorSchemesLocalNames.append(color.baseName());
    }
    QDir colorsSystemDir("/usr/share/color-schemes");
    colorsSystemDir.setNameFilters(QStringList() << "*.colors");
    colorsSystemDir.setFilter(QDir::Files);
    colorsSystemDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesSystem = colorsSystemDir.entryInfoList();
    QStringList colorSchemesSystemNames;
    for (const auto & i : qAsConst(colorSchemesSystem))
    {
        colorSchemesSystemNames.append(i.baseName());
    }
    QStringList colorSchemesNames = colorSchemesSystemNames + colorSchemesLocalNames;
    return colorSchemesNames;
}
