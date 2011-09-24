#ifndef HPSTHREADMANAGER_H
#define HPSTHREADMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include "hpsoption.h"
#include "hpsdbhandler.h"
#include "hpsthumb.h"
class HPSImageLoader;
class HPSDirLister;
class HPSDirManager;
class HPSImageLoader;
class HPSDirChecker;
class HPSDirWatcher;
class HPSKnotDirModel;

class HPSThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit HPSThreadManager(QObject *parent = 0);
    enum ThreadTyp { DirLister,DirChecker,DirWatcher,DirWachterImageLoader,ImageLoader};
    HPSDirLister* dirLister();
    HPSDirChecker* dirChecker();
    HPSDirWatcher* dirWatcher();
    QVector<HPSImageLoader*> &imageLoaders();
    HPSImageLoader * dirWatcherImageLoader();
    void closeAllThreads();
    bool threadClosed();
    void initDirLister(HPSKnotDirModel &knotModel);
    void initDirChecker(HPSOption &option,HPSDBHandler &dbHandler);
    void initImagerLoaders(QVector<HPSThumb> &thumbVec);
    void initDirWatcher(HPSOption &option,HPSDirManager &dirManager,HPSDBHandler &dbHandler);
    void initDirWatcherImageLoader(QVector<HPSThumb> &thumbVec);


signals:
    void  allThreadsAreClosed();
public slots:
private slots:
    void onThreadFinished();
private:


    QVector<QThread*> mThreads;
    QVector<HPSImageLoader*> mImageLoaders;
    HPSDirLister *mDirLister;
    HPSDirChecker *mDirChecker;
    HPSDirWatcher *mDirWatcher;
    HPSImageLoader *mDirWatcherImageLoader;
    bool mThreadsClosed;
};

#endif // HPSTHREADMANAGER_H
