#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QWidget>
#include "ui_generalsettings.h"
#include <QSettings>
//koi

namespace Ui
{
class GeneralSettings;
}

class GeneralSettings: public QWidget
{
Q_OBJECT
public:
	GeneralSettings(QWidget *parent);
	~GeneralSettings() override;

	void saveSettings();
	void loadSettings();
private slots:

private:
	Ui::GeneralSettings *ui;
	QSettings m_settings;
};

#endif // GENERALSETTINGS_H
