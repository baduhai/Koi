//
// Created by da-viper on 18/10/2020.
//

#ifndef EXTERNAL_H
#define EXTERNAL_H

//Qt
#include <QStringList>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

namespace  utils
{
/**when adding other features this is where you put them
 * just to make it neat
 */

//Get the available profile themes
QStringList getKonsoleThemes();

namespace noUse
{
/*** currently there is not way i have found to apply the
 * colourscheme dynamically but may change in the future
 * @param name the name of the colorscheme
 */
void setKonsoleTheme(const QString &name);

QFileInfoList getKonsoleDirPaths();

}
}

#endif //EXTERNAL_H
