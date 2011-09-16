#ifndef HPSDIRCHECKER_H
#define HPSDIRCHECKER_H

#include <QObject>
#include <QtCore>
#include <QDebug>
#include "hpsimageloader.h"
#include "hpsdbhandler.h"
#include "hpsworkerclass.h"
#include "hpshashsaver.h"
#include "hpsthumb.h"
#include "hpsoption.h"
#include "hpsthreadmanager.h"

class HPSDirChecker : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirChecker(HPSThreadManager &threadManager,HPSOption &option,HPSDBHandler *dbHandler,QObject *parent = 0);
    ~HPSDirChecker();
    void close();
  signals:
    void dirChecked(QString dir);

public slots:
    void checkDir(QString dir);

    // void creatThumbs(const QStringList &cDirs);
    //void creatThumbs(const QString &cDir);

    //int  workCount();

    /*


    void startThumbCreation(const QString&,const int);
    void thumbsReady(const int);
    void creationReady();
    void startCreation();
    void dirCreationReady(QString dir);



private slots:
     void fertigTime();
     void fotosReady(int pos, int count);
     void fotoReady();
     void checkIfAllClose();
     void getError(int error);
     void hashesReady();*/

private:

    HPSThreadManager &mThreadManager;
    HPSOption &mOption;
    HPSDBHandler *mDatabaseHandler;
    QString mCurrentDir;
    QVector<HPSThumb> mThumbs;
    bool mShutDown;
    bool mIsRunning;
    bool mIsWorking;
    int mCountError;
    int mThumbsLoaded;
    QStringList mDirQueue;
    void startWork(const QString &cDir,const bool withView);
    void workReady();
    void nextWork();
    /*
    void saveHashes();
    void reset();
    void subDirsFrom(const QString &dir,QStringList &dirs);
    void makeView(const QList<QStringList> &list);

*/
};

#endif // HPSDIRCHECKER_H
