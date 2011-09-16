#include "hpsthreadmanager.h"
#include "hpsdirlister.h"
#include "hpsdirchecker.h"
#include "hpsimageloader.h"

HPSThreadManager::HPSThreadManager(HPSKnotDirModel &knotModel,HPSOption &option,HPSDBHandler *dbHandler,QObject *parent) :
    QObject(parent),mThreads(2+QThread::idealThreadCount()),mThreadsClosed(false)
{


    QThread *t;
    for (int i = 0; i < mThreads.size(); ++i) {
        t = new QThread();
        t->start();
        connect(t,SIGNAL(finished()),this,SLOT(onThreadFinished()));
        mThreads[i] = t;
    }
    initDirLister(knotModel);
    initDirChecker(option,dbHandler);
    initImagerLoaders();
}

void HPSThreadManager::initDirLister(HPSKnotDirModel &knotModel)
{
    mDirLister = new  HPSDirLister(knotModel);
    mDirLister->moveToThread(mThreads[DirLister]);
    connect(mDirLister,SIGNAL(destroyed()),mThreads[DirLister],SLOT(quit()));

}

void HPSThreadManager::initDirChecker(HPSOption &option,HPSDBHandler *dbHandler)
{

    mDirChecker = new HPSDirChecker(*this,option,dbHandler);
    mDirChecker->moveToThread(mThreads[DirChecker]);
    connect(mDirChecker,SIGNAL(destroyed()),mThreads[DirChecker],SLOT(quit()));

}

void HPSThreadManager::initImagerLoaders()
{
    mImageLoaders.resize(QThread::idealThreadCount());
    for (int i = 0; i < mImageLoaders.size(); ++i) {
        mImageLoaders[i] = new HPSImageLoader();
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
    mDirLister->close();
    mDirChecker->close();
    for (int i = 0; i < mImageLoaders.size(); ++i) {
        mImageLoaders[i]->close();

    }
}


