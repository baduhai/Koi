//
// Created by da-viper on 05/09/2020.
//

#include "profile.h"

//may never use this.
Profile::Profile()
    :
    m_name(), m_plasma(), m_color(),
    m_gtk(), m_kvantum(), m_widget(),
    m_icon(), m_script(), m_wallpaper(),
    m_library(), m_theme(), m_konsole()
{
    m_name = "name of profile here...";
}

//copying from and existing profile.
Profile::Profile(const Profile &p)
    :
    m_name(), m_plasma(p.m_plasma), m_color(p.m_color),
    m_gtk(p.m_gtk), m_kvantum(p.m_kvantum), m_widget(p.m_widget),
    m_icon(p.m_icon), m_script(p.m_script), m_wallpaper(p.m_wallpaper),
    m_library(p.m_library), m_theme(p.m_theme), m_konsole(p.m_konsole)
{
    m_name = p.m_name + "-1";
}

void Profile::readConfig(QSettings &s)
{

    //Styles
    s.beginGroup("Styles");
    QString group("Styles");
    m_plasma = s.value("/plasmaStyle").toString();
    m_color = s.value("/colorScheme").toString();
    m_gtk = s.value("/gtkTheme").toString();
    m_kvantum = s.value("/kvantum").toString();
    m_widget = s.value("/widgetStyle").toString();
    s.endGroup();

    //Others;
    s.beginGroup("Others");
    m_icon = s.value("icon").toString();
    m_mouse = s.value("mouse").toString();
    m_script = s.value("script").toString();
    m_wallpaper = s.value("wallpaper").toString();
    s.endGroup();

    //Window Decoration
    s.beginGroup("Window Decoration");
    m_library = s.value("library").toString();
    m_theme = s.value("theme").toString();
    s.endGroup();

    //External
    s.beginGroup("External");
    m_konsole = s.value("konsole").toString();
    s.endGroup();

}

void Profile::writeConfig(QSettings &s)
{

    //void write to globalTheme.
    //Styles
    s.beginGroup("Style");
    s.setValue("plasmaStyle", m_plasma);
    s.setValue("colorScheme", m_color);
    s.setValue("gtkTheme", m_gtk);
    s.setValue("kvantum", m_kvantum);
    s.setValue("widgetStyle", m_widget);
    s.endGroup();

    //Others
    s.beginGroup("Others");
    s.setValue("icon", m_icon);
    s.setValue("mouse", m_mouse);
    s.setValue("script", m_script);
    s.setValue("wallpaper", m_wallpaper);
    s.endGroup();

    //Window Decorations;
    s.beginGroup("Window Decoration");
    s.setValue("library ", m_library);
    s.setValue("theme", m_theme);
    s.endGroup();

    //External
    s.beginGroup("External");
    s.setValue("konsole", m_konsole);
}

/*  data is never read from global
    *this is only for inbuilt kde configs
    *theme global configs is always prefixed with Koi-$nameoftheme
*/
void Profile::writeToGlobal()
{
    QString pluginName("Koi-" + m_name);
    //write the defaults file, read from kde config files and save to the defaultsrc
    KConfig defaultsConfig
        (QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) %
            QLatin1String("/plasma/look-and-feel/")
             % pluginName % "/contents/defaults");

    KConfigGroup defaultsConfigGroup(&defaultsConfig, "kdeglobals");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "KDE");

    //widget style
    defaultsConfigGroup.writeEntry("widgetStyle", m_widget);

    //icon style
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kdeglobals");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Icons");
    defaultsConfigGroup.writeEntry("Theme", m_icon);

    //colour style
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kdeglobals");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "General");
    defaultsConfigGroup.writeEntry("ColorScheme", m_color);

    //plasma theme
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "plasmarc");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Theme");
    defaultsConfigGroup.writeEntry("name", m_plasma);

    //cursor theme
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kcminputrc");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Mouse");
    defaultsConfigGroup.writeEntry("cursorTheme", m_mouse);

    //window decoration
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kwinrc");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "org.kde.kdecoration2");
    defaultsConfigGroup.writeEntry("library", m_library);
    defaultsConfigGroup.writeEntry("theme", m_theme);
}

void Profile::setName(const QString &name)
{
    m_name = name;
}
QString Profile::name() const
{
    return m_name;
}