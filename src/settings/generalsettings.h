#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QWidget>
#include "ui_generalsettings.h"

namespace Ui {
class GeneralSettings;
}

class GeneralSettings : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralSettings(QWidget *parent = nullptr);
    ~GeneralSettings() override;

private:
    Ui::GeneralSettings *ui;
};

#endif // GENERALSETTINGS_H
