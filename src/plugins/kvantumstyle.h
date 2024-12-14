#pragma once

#include "headers/plugin.h"

class KvantumStyle : ProcessPlugin {
public:
  void setTheme(QString kvantumStyle) override;
  QStringList getThemes() override;
};
