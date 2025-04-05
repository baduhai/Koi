#pragma once

#include "headers/plugin.h"

class Script : Plugin {
public:
  void setTheme(QString scriptFile) override;
};
