#include "hpsthreadmanager.h"
#include "hpsdirlister.h"
#include "hpsdirchecker.h"
#include "hpsimageloader.h"
#include "hpicsync.h"
#include "hpsdirwatcher.h"


HPSThreadManager::HPSThreadManager(QObject *parent) :
    QObject(parent),mThreads(4+QThread::idealThreadCount()),mThreadsClosed(false)
{

    QThread *t;
    for (int i = 0; i < mThreads.size(); ++i) {
        t = new QThread();
        t->start();
        connect(t,SIGNAL(finished()),this,SLOT(onThreadFinished()));
        mThreads[i] = t;
    }
}

void HPSThreadManager::initDirLister(HPSKnotDirModel &knotModel)
{
    mDirLister = new  HPSDirLister(knotModel);
    mDirLister->moveToThread(mThreads[DirLister]);
    connect(mDirLister,SIGNAL(destroyed()),mThreads[DirLister],SLOT(quit()));

}

void HPSThreadManager::initDirChecker(HPSOption &option,HPSDBHandler &dbHandler)
{
HPicSync * p =static_cast<HPicSync*>(parent());
qDebug() << Q_FUNC_INFO <<p;
    mDirChecker = new HPSDirChecker(p->tmpListWidgetItems(),*this,option,dbHandler);

    mDirChecker->moveToThread(mThreads[DirChecker]);
    connect(mDirChecker,SIGNAL(destroyed()),mThreads[DirChecker],SLOT(quit()));
    connect(mDirChecker,SIGNAL(newItemListWidgtesReady(int,int)),p,SLOT(onImageLoaderThumbsReady(int,int)));
    connect(mDirChecker,SIGNAL(startCheckOn(QString)),p,SLOT(onDirCheckerStartCheck(QString)));
    connect(mDirChecker,SIGNAL(finished()),p,SLOT(onDirCheckerFinished()));

}

void HPSThreadManager::initImagerLoaders(QVector<HPSThumb> &thumbVec)
{

    mImageLoaders.resize(QThread::idealThreadCount());
    for (int i = 0; i < mImageLoaders.size(); ++i) {
        mImageLoaders[i] = new HPSImageLoader(thumbVec);
        mImageLoaders[i]->moveToThread(mThreads[ImageLoader+i]);
        connect(mImageLoaders[i],SIGNAL(destroyed()),mThreads[ImageLoader+i],SLOT(quit()));
    }
}


HPSDirLister * HPSThreadManager::dirLister()
{
    return mDirLister;
}

HPSDirChecker * HPSThreadManager::dirChecker()
{
    return mDirChecker;
}

QVector<HPSImageLoader *> & HPSThreadManager::imageLoaders()
{
    return mImageLoaders;
}

bool HPSThreadManager::threadClosed()
{
    return mThreadsClosed;
}

void HPSThreadManager::onThreadFinished()
{
    QThread *thread = (QThread*)sender();
    for (int i = 0; i < mThreads.size(); ++i) {
        if( mThreads.at(i) == thread){
            mThreads.remove(i);
            break;
        }
    }
    delete thread;
    if(mThreads.empty()){
        mThreadsClosed = true;
        emit allThreadsAreClosed();
    }
}

void HPSThreadManager::closeAllThreads()
{

    disconnect(mDirLister,SIGNAL(dirDone(QString)),0,0);
    mDirChecker->disconnect(mDirLister);
    disconnect(mDirChecker,SIGNAL(startImageloaders()));
    mDirLister->close();
    mDirChecker->close();
    mDirWatcher->close();
    mDirWatcherImageLoader->close();
    for (int i = 0; i < mImageLoaders.size(); ++i) {       
        mImageLoaders[i]->close();

    }

}

void HPSThreadManager::initDirWatcher(HPSOption &option,HPSDirManager &dirManager, HPSDBHandler &dbHandler)
{
    mDirWatcher = new HPSDirWatcher(*this,option,dirManager,dbHandler);
    mDirWatcher->moveToThread(mThreads[DirWatcher]);
    connect(mDirWatcher,SIGNAL(destroyed()),mThreads[DirWatcher],SLOT(quit()));
}

HPSDirWatcher * HPSThreadManager::dirWatcher()
{
    return mDirWatcher;
}

void HPSThreadManager::initDirWatcherImageLoader(QVector<HPSThumb> &thumbVec)
{
    mDirWatcherImageLoader = new HPSImageLoader(thumbVec);
    mDirWatcherImageLoader->moveToThread(mThreads[DirWachterImageLoader]);
    connect(mDirWatcherImageLoader,SIGNAL(destroyed()),mThreads[DirWachterImageLoader],SLOT(quit()));
}

HPSImageLoader * HPSThreadManager::dirWatcherImageLoader()
{
    return mDirWatcherImageLoader;
}


