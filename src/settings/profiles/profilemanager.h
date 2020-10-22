//
// Created by da-viper on 06/09/2020.
//

#ifndef KOI_PROFILEMANAGER_H
#define KOI_PROFILEMANAGER_H

//Qt
#include <QString>
#include <QDebug>
#include <KSharedConfig>
#include <QPointer>

//koi
#include "profile.h"

class ProfileManager: public QObject
{
Q_OBJECT
Q_SIGNALS:
    void favouritesChanged(const QString &profileName, const bool &isFav);
public:
    ProfileManager();
    ~ProfileManager() override;
    static ProfileManager *instance();

    bool profileExists(const QString &fileName);

    //TODO fix favourites
    //loads all profile including the defaults.
    QList<Profile *> allProfiles();
    QHash<QString, QString> getFavouritesList() const;
    QStringList favouriteNames() const;
    void saveProfile(const QString &profileName);

    Profile *_activeProfile{};

    bool addProfile(Profile *profile);
    //can only delete the active profile.
    void deleteProfile();
    bool isFavourite(const QString &profileName);
    Profile *getProfile(const QString &profileName);
    void setFavourite(const QString &profileName, bool favourite);
    void saveFavourites();
private:

    //load the profile to the qhash. hashed with their filename
    void loadProfiles();

    //////Name-----time
    QHash<QString, QString> m_favourites;

    //get the list of profiles
    QFileInfoList listProfiles();

    void loadFavourites();

    //for storing the loaded profiles.
    QHash<QString, Profile *> _profileList;
    bool m_loadedAllProfiles{};
    //makes sure it is only one instance of the class
    //this is loaded when you use koi for the first time
    // or don't have and profile set
    Q_DISABLE_COPY(ProfileManager);
};


#endif //KOI_PROFILEMANAGER_H
