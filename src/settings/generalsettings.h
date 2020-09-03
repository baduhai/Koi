#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QWidget>
#include "ui_generalsettings.h"
#include <QSettings>
//koi

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
	void loadSetting();
private slots:

private:
    Ui::GeneralSettings *ui;
    QSettings *setting;
};

#endif // GENERALSETTINGS_H
