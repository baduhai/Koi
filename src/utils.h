#ifndef UTILS_H
#define UTILS_H
// Headers
#include "styles.h"
#include "others.h"
#include "external.h"

// Qt libs
#include <QtGlobal>
#include <QObject>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QDateTime>
#include <QTimer>
#include <QTest>
#include <settings/profiles/profile.h>

// KDE libs
#include <KCoreAddons>


namespace utils
{

void notify(QString notifySummary = "", QString notifyBody = "", int timeoutms = 5000);
QString startupTimeCheck();

namespace noUse
{
//this would be used for global theme
void useGlobalTheme(const QString &pluginName);
}

void go(Profile *p);
}
#endif // UTILS_H
