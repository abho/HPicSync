#ifndef HPSDIRCHECKER_H
#define HPSDIRCHECKER_H

#include <QObject>
#include <QtCore>
#include <QDebug>
#include "hpsimageloader.h"
#include "hpsdbhandler.h"
#include "hpsthumb.h"
#include "hpsoption.h"
#include "hpsthreadmanager.h"

class HPSDirChecker : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirChecker(QVector<QListWidgetItem*> &vec,HPSThreadManager &threadManager,HPSOption &option,HPSDBHandler &dbHandler,QObject *parent = 0);
    ~HPSDirChecker();
    void close();
  signals:
    void dirChecked(QString dir);
    void startImageloaders();
    void newItemListWidgtesReady(int pos, int count);


public slots:
    void checkDir(QString dir);
     void fotosReady(int pos, int count);
     void imageLoaderFertig();
     void imageLoaderError(int errorPos);
     void onImageLoaderDestroyed();

private:

    QVector<QListWidgetItem*> &mTmpListWidgetItems;
    HPSThreadManager &mThreadManager;
    HPSOption &mOption;
    HPSDBHandler &mDatabaseHandler;
    QString mCurrentDir;
    QVector<HPSThumb> mThumbs;    
    int mCountError;
    int mThumbsLoaded;
    int mCountDestroyedImageLoader;
    bool mShutDown;
    bool mIsRunning;
    bool mIsWorking;
    QPair<int, int> mCountImageLoader;
    QStringList mDirQueue;


    void startWork(const QString &cDir,const bool withView);
    void workReady(const QString &dir);
    void nextWork();
    bool saveThumbsToDB(const QString &dir);



};

#endif // HPSDIRCHECKER_H
