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
    const QStringList & dirs();
    void setDirs(const QStringList &list);
    void addDir(const QString &dir);
    void appendDirs(const QStringList &list);
    void removeDir(const QString &dir);
    void removeDirs(const QStringList &dirs);
    int getComboBoxView();
    void setComboBoxView(int view);
    const QString & getComboBoxCurrentDir();
    void setComboBoxCurrentDir(const QString &currentDir);
    void setExpandDirs(const QStringList &dirs);
    const QStringList& expandDirs();
    const QStringList& createThumbDirs();
    void setCreateThumbDirs(const QStringList &dirs);
    void removeCreateThumbDir(const QString &dir);
    void addCreateThumbDir(const QString &dir);
    void appendCreateThumbDir(const QStringList &dirs);
    void setDirFromDirlister(const QString &dir);
    void addDeleteDir(const QString &dir);
    void removeDeleteDir(const QString &dir);
    const QStringList & deleteDirs();
    const QString & dirFromDirlister();


    void test(const QString &);


private:
    QString mComboBoxCurrentDir;
    QSettings mSetting;
    QString mQuellOrdner,mUsername,mPassword;
    QString mDirFromDirlister;
    int mThumbSize,mPort;
    QRect mGeometry,mGeometryOption;
    QStringList mOrdner;
    QStringList mExpandDirs;
    QStringList mCreateThumbDirs;
    QStringList mDeleteDirs;
    int mComboBoxView;
};

#endif // HPSOPTION_S
