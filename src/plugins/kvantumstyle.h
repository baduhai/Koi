#pragma once

#include "src/headers/plugin.h"

class KvantumStyle : Plugin {
public:
  void setTheme(QString kvantumStyle) override;
  QStringList getThemes() override;
};
