#pragma once

#include "headers/plugin.h"

class KvantumStyle : Plugin {
public:
  void setTheme(QString kvantumStyle) override;
  QStringList getThemes() override;
};
