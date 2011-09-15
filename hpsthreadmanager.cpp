#include "hpsthreadmanager.h"
#include "hpsdirlister.h"
#include "hpsdirchecker.h"
#include "hpsimageloader.h"

HPSThreadManager::HPSThreadManager(HPSKnotDirModel &knotModel,HPSOption &option,HPSDBHandler *dbHandler,QObject *parent) :
    QObject(parent)/*,mThreads(QThread::idealThreadCount())*/
{
    initDirLister(knotModel);
    initDirChecker(option,dbHandler);
}

void HPSThreadManager::initDirLister(HPSKnotDirModel &knotModel)
{
   /* mDirLister = new  HPSDirLister(knotModel);
    mDirLister->moveToThread(&mThreads[DirLister]);
    connect(mDirLister,SIGNAL(destroyed()),&mThreads[DirLister],SLOT(quit()));
*/
}

void HPSThreadManager::initDirChecker(HPSOption &option,HPSDBHandler *dbHandler)
{
    mThreads.append(QThread());
    HPSDirChecker *dirChecker = new HPSDirChecker(option,dbHandler);
    QThread *t =  new QThread();
    dirChecker->moveToThread(t);
    const QThread &th=mThreads[0];
    //connect(dirChecker,SIGNAL(destroyed()),&mThreads[DirChecker],SLOT(quit()));

}

void HPSThreadManager::initImagerLoaders()
{
   /* mImageLoaders.resize(QThread::idealThreadCount());
    for (int i = 0; i < mImageLoaders.size(); ++i) {
        mImageLoaders[i] = new HPSImageLoader();
        mImageLoaders[i]->moveToThread(&mThreads[ImageLoader+i]);
        connect(mImageLoaders[i],SIGNAL(destroyed()),&mThreads[ImageLoader+i],SLOT(quit()));
    }*/
}
