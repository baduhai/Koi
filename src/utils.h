#ifndef UTILS_H
#define UTILS_H
// Headers
#include "wallpaper.h"
#include "kvantumstyle.h"

// Qt libs
#include <QtGlobal>
#include <QObject>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QDateTime>
#include <QTimer>
#include <QTest>
#include <QtWidgets/QStyleFactory>
#include <settings/profiles/profile.h>

struct Decoration
{
	QString library;
	QString theme;
	QString name;
};
class Utils: public QObject
{
	Q_OBJECT
public:

	explicit Utils(Profile *pProfile);

	Utils();

	//styles.
	static QStringList getPlasmaStyles();
	static QStringList getColorSchemes();
	static QStringList getGtkThemes();
	static QStringList getKvantumStyles();
	static QStringList getWidgetStyles();
	static QStringList getIcons();
	static QStringList getCursorThemes();

	static QStringList getWindowDecorationsStyle();
	static QList<Decoration> getWindowDecorations();

	void notify(QString notifySummary = "", QString notifyBody = "", int timeoutms = 5000);
	static QString startupTimeCheck();
	//this would be used for global theme
	void useGlobalTheme();
	void go();
	void goKvantumStyle();
	void goWall();

	void runScript();
	static bool themeExists(const QString &themeName);

private:
	void setGtk(const QString &gtkTheme);
	KvantumStyle kvantumStyle;
	Wallpaper wallpaper;

	QDBusConnection *bus{};

	QDBusInterface *notifyInterface{};
	Profile *_profile;
};

#endif // UTILS_H
