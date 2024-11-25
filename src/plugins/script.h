#pragma once

#include "headers/plugin.h"

class Script : ProcessPlugin {
public:
  void setTheme(QString scriptFile) override;
};
