#pragma once

#include "headers/plugin.h"
class PlasmaStyle : Plugin {
public:
  void setTheme(QString plasmaStyle) override;
  QStringList getThemes() override;
};
