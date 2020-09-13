#ifndef UTILS_H
#define UTILS_H
// Headers
#include "colorscheme.h"
#include "gtk.h"
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

	Utils();

	QSettings *settings{};
	void initialiseSettings();

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
	void startupTimeCheck();
	//this would be used for global theme
	void useGlobalTheme(QString type);
	void go(const QString &themeType);
	void goColors(const QString &themeType);
	void goGtk(const QString &themeType);
	void goKvantumStyle(const QString &themeType);
	void goWall(const QString &themeType);

	void runScript(const QString &themeType);
	static bool themeExists(const QString &themeName);

private:

	Gtk gtk;
	KvantumStyle kvantumStyle;
	Wallpaper wallpaper;

	ColorScheme colorScheme;

	QProcess *useGlobalProcess{};

	QDBusConnection *bus{};

	QDBusInterface *notifyInterface{};

};

#endif // UTILS_H
