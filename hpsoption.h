#ifndef HPSOPTION_S
#define HPSOPTION_S


#include <QSettings>
#include <QtCore>
class HPSOption
{
public:
    enum ComboBoxView {ListView,TreeView};

    explicit HPSOption();
    const QString & getQuellOrdner();
    int getThumbSize();
    const QRect & getGeometry();
    const QRect & getGeometryOption();
    void setQuellOrdner(const QString &quellOrdner);
    void setThumbSize(const int size);
    void setGeometry(const QRect &geometry);
    void setGeometryOption(const QRect &geometry);
    const QString & getUsername();
    void setUsername(const QString &username);
    const QString & getPassword();
    void setPassword(const QString &password);
    int getPort();
    void setPort(int port);
    const QStringList & getOrdner();
    void setOrdner(const QStringList &list);
    void addOrdner(const QString &dir);
    void removeOrdner(const QString &dir);
    int getComboBoxView();
    void setComboBoxView(int view);
    const QString & getComboBoxCurrentDir();
    void setComboBoxCurrentDir(const QString &currentDir);
    void setExpandDirs(const QStringList &dirs);
    const QStringList& expandDirs();


private:
    QString mComboBoxCurrentDir;
    QSettings mSetting;
    QString mQuellOrdner,mUsername,mPassword;
    int mThumbSize,mPort;
    QRect mGeometry,mGeometryOption;
    QStringList mOrdner;
    QStringList mExpandDirs;
    int mComboBoxView;
};

#endif // HPSOPTION_S
