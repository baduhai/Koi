#include "wallpaper.h"

void Wallpaper::setTheme(QString wallFile) {

  if(!interface)
  {
    interface = new QDBusInterface("org.kde.plasmashell", "/PlasmaShell",
                        "org.kde.PlasmaShell", QDBusConnection::sessionBus());
  }
  QString script = "var allDesktops = desktops();";
  script += "print (allDesktops);";
  script += "for (i=0;i<allDesktops.length;i++) {";
  script += "d = allDesktops[i];";
  script += "d.wallpaperPlugin = \"org.kde.image\";";
  script += "d.currentConfigGroup = Array(\"Wallpaper\", \"org.kde.image\", "
            "\"General\");";
  script += "d.writeConfig(\"Image\", \"file://";
  script += wallFile;
  script += "\")}";
  interface->call("evaluateScript", script);
}
