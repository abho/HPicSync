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

    // void creatThumbs(const QStringList &cDirs);
    //void creatThumbs(const QString &cDir);

    //int  workCount();
    //bool startWork();
    /*
signals:

    void startThumbCreation(const QString&,const int);
    void thumbsReady(const int);
    void creationReady();
    void startCreation();
    void dirCreationReady(QString dir);

public slots:



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
    int mCountError;
    int mThumbsLoaded;
    QStringList mDirQueue;
    /*
    void saveHashes();
    void reset();
    void makeThumbsAndView(const QString &cDir,const bool withView);
    void subDirsFrom(const QString &dir,QStringList &dirs);
    void makeView(const QList<QStringList> &list);
    void nextWork();
*/
};

#endif // HPSDIRCHECKER_H
