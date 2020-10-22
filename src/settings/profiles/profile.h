//
// Created by da-viper on 05/09/2020.
//

#ifndef KOI_PROFILE_H
#define KOI_PROFILE_H

#include <QString>
#include <QSettings>
#include <KConfig>
#include <KConfigGroup>
#include <QDir>
#include <KSharedConfig>

class Profile: public QObject
{
Q_OBJECT
public:
    Profile();
    Profile(const Profile &p);
    ~Profile();

    void readConfig(QSettings &s);

    void writeConfig(QSettings &s) const;

    //cannot use qsetting here because of double grouping in defaults file ;
    void writeToGlobal() const;

    // Sets the Profile Name
    void setName(const QString &name);
    //QString m_browser;
    void createProfileGlobalDir() const;

    //QString m_vscode;
    [[nodiscard]] bool globalDefaultExists() const;
    // Returns the profile Name
    QString name() const;
    QString pluginName() const;
    //getters and setters for the variables
    //getters
    QString getGlobDir() const;
    QString configPath() const;
    QString getPlasma() const;
    QString getColor() const;
    QString getGtk() const;
    QString getKvantum() const;
    QString getWidget() const;
    //Others
    QString getIcon() const;
    QString getMouse() const;
    QString getScript() const;
    [[nodiscard]] bool getScriptEnabled() const;
    QVariant getWallpaper() const;
    [[nodiscard]] bool getWallEnabled() const;
    //Window Decorations
    QString getDecName() const;
    //External
    QString getKonsole() const;
    [[nodiscard]] bool getKonsoleEnabled() const;

    //Maybe i don't need setters ?
    //Setters
    void setGlobDir();
    void setConfigPath();
    void setPlasma(const QString &plasma);
    void setColor(const QString &color);
    void setGtk(const QString &gtk);
    void setKvantum(const QString &kvantum);
    void setWidget(const QString &widget);
    void setIcon(const QString &icon);
    void setMouse(const QString &mouse);
    void setScriptEnabled(const bool &scriptEnabled);
    void setScript(const QString &script);
    void setWallEnabled(const bool &wallEnabled);
    void setWallpaper(QVariant wallPaper);
    void setDecName(const QString &decorationName);
    void setLibrary(const QString &library);
    void setTheme(const QString &theme);
    void setKonsole(const QString &konsole);
    void setKonsoleEnabled(const bool &konsoleEnabled);
private:

    //name of the profile that will be used for its location
    QDir m_globDir;
    QString m_configPath;

    QString m_name;
    //can never set the plugin in name manually;
    QString m_pluginName;
    //Group
    //[Styles]
    QString m_plasma;
    QString m_color;
    QString m_gtk;

    QString m_kvantum;
    QString m_widget;
    //[Others]
    QString m_icon;
    QString m_mouse;

    QString m_script;
    bool m_scriptEnabled;
    QVariant m_wallpaper;
    bool m_wallEnabled;

    //[Window Decorations];
    QString m_decorationName;
    QString m_library;
    QString m_theme;

    //External ;
    QString m_konsole;
    bool m_konsoleEnabled;
};


#endif //KOI_PROFILE_H
