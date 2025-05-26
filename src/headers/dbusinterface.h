#include <QObject>
#include "utils.h"

class KoiDbusInterface : public QObject {
    Q_OBJECT
    Q_CLASSINFO("Koi D-Bus Interface", "dev.baduhai.Koi")

    public:
        KoiDbusInterface(QObject* ,Utils* );
        ~KoiDbusInterface();

    public Q_SLOTS:
        void goLightMode();
        void goDarkMode();
        void toggleMode();

    private:
        Utils* utils;
};
