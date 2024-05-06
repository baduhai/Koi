#ifndef ABOUT_H
#define ABOUT_H
// Headers
#include "license.h"
// Qt libs
#include <QDialog>
// KF libs
#include <KCoreAddons>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = nullptr);
    ~About();

private slots:
    void on_closeBtn_clicked();
    void on_licenseBtn_clicked();
    void on_wWebsiteBtn_clicked();

private:
    Ui::About *ui;
};

#endif // ABOUT_H
