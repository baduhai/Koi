#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QWidget>
#include "ui_generalsettings.h"

//koi
#include "utils.h"

namespace Ui {
class GeneralSettings;
}

class GeneralSettings : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralSettings(QWidget *parent = nullptr);
    ~GeneralSettings() override;

private slots:
    void enableNotification(bool enable);

private:
    Ui::GeneralSettings *ui;
    Utils utils;
};

#endif // GENERALSETTINGS_H
