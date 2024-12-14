#pragma once

#include "headers/plugin.h"
class Wallpaper : DbusPlugin {
public:
  void setTheme(QString wallFile) override;
};
