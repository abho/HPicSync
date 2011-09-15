#ifndef HPSTHREADMANAGER_H
#define HPSTHREADMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include "hpsworkerclass.h"
#include "hpsoption.h"
#include "hpsdbhandler.h"

class HPSImageLoader;
class HPSDirLister;
class HPSKnotDirModel;
class HPSImageLoader;
class HPSDirChecker;
class HPSThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit HPSThreadManager(HPSKnotDirModel &knotModel,HPSOption &option,HPSDBHandler *dbHandler,QObject *parent = 0);
    enum ThreadTyp { DirLister,DirChecker,ImageLoader};
    HPSDirLister* dirLister();
    HPSDirChecker* dirChecker();
    QVector<HPSImageLoader*> &imageLoaders();
    void closeAllThreads();
    bool threadClosed();

signals:
    void  allThreadsAreClosed();
public slots:
private slots:
    void onThreadFinished();
private:
    void initDirLister(HPSKnotDirModel &knotModel);
    void initDirChecker(HPSOption &option,HPSDBHandler *dbHandler);
    void initImagerLoaders();


    QVector<QThread*> mThreads;
    QVector<HPSImageLoader*> mImageLoaders;
    HPSDirLister *mDirLister;
    HPSDirChecker *mDirChecker;
    bool mThreadsClosed;
};

#endif // HPSTHREADMANAGER_H
