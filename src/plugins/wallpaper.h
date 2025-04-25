#pragma once

#include "src/headers/plugin.h"
class Wallpaper : DbusPlugin {
public:
  void setTheme(QString wallFile) override;
};
