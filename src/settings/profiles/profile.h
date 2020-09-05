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

class Profile {

public:
    Profile();
    Profile(const Profile &p );
    ~Profile();

    void readConfig(QSettings &s);

    void writeConfig(QSettings &s);

    //cannot use qsetting here because of double grouping in defaults file ;
    void writeToGlobal();

    // Sets the Profile Name
    void setName(const QString &name);
    // Returns the profile Name
    QString name() const;


private:
    //name of the profile that will be used for its location
    QString m_name  ;

    //Group
    //[Styles]
    QString m_plasma;
    QString m_color;
    QString m_gtk;
    QString m_kvantum ;
    QString m_widget;

    //[Others]
    QString m_icon;
    QString m_mouse;
    QString m_script;
    QString m_wallpaper;

    //[Window Decorations];
    QString m_library;
    QString m_theme;

    //External ;
    QString m_konsole;
    //QString m_vscode;
    //QString m_browser;
};


#endif //KOI_PROFILE_H
