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
	GeneralSettings(QWidget *parent, QSettings *pSettings);
	~GeneralSettings() override;

	void applySetting();

private slots:

private:
    Ui::GeneralSettings *ui;
    Utils utils;
    QSettings *setting;
};

#endif // GENERALSETTINGS_H
