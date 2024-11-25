#pragma once

#include "headers/plugin.h"
class PlasmaStyle : ProcessPlugin {
public:
  void setTheme(QString plasmaStyle) override;
};
