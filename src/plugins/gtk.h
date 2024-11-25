#pragma once

#include "headers/plugin.h"
class Gtk : DbusPlugin {
public:
  void setTheme(QString theme) override;
};
