#ifndef LNFLOGIC_H
#define LNFLOGIC_H
#include <QAbstractListModel>
#include <KPackage/Package>
class LnfListModel;
class LnfLogic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(LnfListModel *lnfList READ lnfList CONSTANT)
    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(bool isWritable READ isWritable NOTIFY themeChanged)
    Q_PROPERTY(QString themeFolder READ themeFolder NOTIFY themeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(QString website READ website WRITE setWebsite NOTIFY websiteChanged)
    Q_PROPERTY(QString license READ license WRITE setLicense NOTIFY licenseChanged)
    Q_PROPERTY(QString thumbnailPath READ thumbnailPath NOTIFY themeChanged)
    Q_PROPERTY(bool performLayoutDump READ performLayoutDump WRITE setPerformLayoutDump NOTIFY performLayoutDumpChanged)
    Q_PROPERTY(bool performDefaultsDump READ performDefaultsDump WRITE setPerformDefaultsDump NOTIFY performDefaultsDumpChanged)
    Q_PROPERTY(bool needsSave READ needsSave NOTIFY needsSaveChanged)
public:
    enum ErrorLevel {
        Info,
        Error
    };
    Q_ENUMS(ErrorLevel)
    explicit LnfLogic(QObject *parent = nullptr);
    ~LnfLogic() override;
    LnfListModel *lnfList();
    void setTheme(const QString &theme);
    QString theme() const;
    bool isWritable() const;
    QString themeFolder() const;
    QString name() const;
    void setName(const QString &name);
    QString comment() const;
    void setComment(const QString &comment);
    QString author() const;
    void setAuthor(const QString &author);
    QString email() const;
    void setEmail(const QString &email);
    QString version() const;
    void setVersion(const QString &version);
    QString website() const;
    void setWebsite(const QString &website);
    QString license() const;
    void setLicense(const QString &license);
    bool performLayoutDump() const;
    void setPerformLayoutDump(bool dump);
    bool performDefaultsDump() const;
    void setPerformDefaultsDump(bool dump);
    QString thumbnailPath() const;
    void dumpPlasmaLayout(const QString &pluginName);
    bool needsSave();
    Q_INVOKABLE void save();
    Q_INVOKABLE void createNewTheme(const QString &pluginName, const QString &name, const QString &comment, const QString &author, const QString &email, const QString &license, const QString &website);
    Q_INVOKABLE void processThumbnail(const QString &path);
    Q_INVOKABLE QString openFile();
    void dumpCurrentPlasmaLayout();
    void dumpDefaultsConfigFile(const QString &pluginName);
Q_SIGNALS:
    void themeChanged();
    void messageRequested(ErrorLevel level, const QString &message);
    void needsSaveChanged();
    void nameChanged();
    void commentChanged();
    void authorChanged();
    void emailChanged();
    void versionChanged();
    void websiteChanged();
    void licenseChanged();
    void performLayoutDumpChanged();
    void performDefaultsDumpChanged();
private:
    QString m_themeName;
    KPackage::Package m_package;
    LnfListModel *m_lnfListModel;
    QHash<QString, QString> m_tempMetadata;
    bool m_performLayoutDump : 1;
    bool m_performDefaultsDump : 1;
    bool m_needsSave;
};
#endif // LNFLOGIC_H