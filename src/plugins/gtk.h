#pragma once

#include "src/headers/plugin.h"

class Gtk : DbusPlugin {
public:
  void setTheme(QString theme) override;
  QStringList getThemes() override;
};
