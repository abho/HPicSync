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

class HPSDirChecker : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirChecker(HPSOption &option,HPSDBHandler *dbHandler,QObject *parent = 0);
    void creatThumbs(const QStringList &cDirs);
    void creatThumbs(const QString &cDir);
    bool allThreadsClose();
    void closeAllThreads();
    int workCount();
    bool startWork();

signals:
    void allThreadsDestroyed();
    void startThumbCreation(const QString&,const int);
    void thumbsReady(const int);
    void creationReady();
    void startCreation();
    void dirCreationReady(QString dir);

public slots:



private slots:
     void fertigTime();
     void threadClear();
     void fotosReady(int pos, int count);
     void fotoReady();
     void checkIfAllClose();
     void getError(int error);
     void hashesReady();

private:
    HPSOption &mOption;
    HPSDBHandler *mDatabaseHandler;

    QString mCurrentDir;
    QVector<HPSThumb> mThumbs;
    QMap<QThread *,HPSWorkerClass *> mThreads;
    bool mEx;
    int mCountError;
    int mThumbsLoaded;

    void saveHashes();
    void  reset();
    void  makeThumbsAndView(const QString &cDir,const bool withView);
    void subDirsFrom(const QString &dir,QStringList &dirs);
    void makeView(const QList<QStringList> &list);
    void nextWork();

};

#endif // HPSDIRCHECKER_H
