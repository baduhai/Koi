#pragma once

#include "headers/plugin.h"

class Icons : Plugin {
public:
  void setTheme(QString iconTheme) override;
  QStringList getThemes() override;
};
