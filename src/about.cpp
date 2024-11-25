#include "headers/about.h"
#include "config.h"
#include "headers/license.h"
#include "ui/ui_about.h"

#include <QDesktopServices>
#include <QUrl>

About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About) {
  ui->setupUi(this);
  this->setFixedSize(505, 330);
  QApplication::setApplicationName(PROJECT_NAME);
  ui->titleLabel->setText(QApplication::applicationName());
  QString pgrmVersion = "Version ";
  QApplication::setApplicationVersion(PROJECT_VER);
  ui->verLabel->setText(pgrmVersion + QApplication::applicationVersion());
  QString qtVersion = "<ul><li>Qt ";
  qtVersion += qVersion();
  qtVersion += "</li></ul>";
  ui->qtVerLabel->setText(qtVersion);
  ui->qtVerLabel->setTextFormat(Qt::RichText);
  QString kfVersion = "<ul><li>KDE Frameworks ";
  kfVersion += KCoreAddons::versionString();
  kfVersion += "</li></ul>";
  ui->kfVersion->setText(kfVersion);
  ui->kfVersion->setTextFormat(Qt::RichText);
  QString bosmaLabel =
      "<ul><li><a href=\"https://github.com/Bosma/Scheduler\">Bosma "
      "Scheduler</a></li></ul>";
  ui->bosmaLib->setText(bosmaLabel);
  ui->bosmaLib->setTextFormat(Qt::RichText);
  ui->bosmaLib->setOpenExternalLinks(1);
  QString ctplLabel =
      "<ul><li><a href=\"https://github.com/vit-vit/CTPL\">CTPL</a></li></ul>";
  ui->ctpLib->setText(ctplLabel);
  ui->ctpLib->setTextFormat(Qt::RichText);
  ui->ctpLib->setOpenExternalLinks(1);
  QString sunriseLabel =
      "<ul><li><a "
      "href=\"https://github.com/signetica/SunRise\">SunRise</a></li></ul>";
  ui->sunriseLib->setText(sunriseLabel);
  ui->sunriseLib->setTextFormat(Qt::RichText);
  ui->sunriseLib->setOpenExternalLinks(1);
  ui->tabWidget->setCurrentIndex(0);
}

About::~About() { delete ui; }

void About::on_closeBtn_clicked() { this->setVisible(0); }

void About::on_licenseBtn_clicked() {
  auto *licence = new License(this);
  licence->open();
}

void About::on_wWebsiteBtn_clicked() {
  QDesktopServices::openUrl(
      QUrl("https://github.com/baduhai", QUrl::TolerantMode));
}
