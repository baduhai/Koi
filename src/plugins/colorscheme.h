#pragma once

#include "headers/plugin.h"

class ColorScheme : protected ProcessPlugin {
public:
  void setTheme(QString themeName) override;
};
