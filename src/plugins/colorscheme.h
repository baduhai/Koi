#pragma once

#include "headers/plugin.h"

class ColorScheme : protected Plugin {
public:
  void setTheme(QString themeName) override;
  QStringList getThemes() override;
};
