#include "hpsdirwatcher.h"

HPSDirWatcher::HPSDirWatcher(HPSThreadManager &threadManager,HPSOption &option,HPSDirManager &dirManager,HPSDBHandler &dbhandler,QObject *parent) :
    QObject(parent),mDBHandler(dbhandler),mOption(option),mDirManager(dirManager),mNextDir(""),mCurrentWatched(""),
    mShutDown(false),mIsRunning(false),mIsImageLoaderRunning(false),mIsWatching(false)
{
    threadManager.initDirWatcherImageLoader(mThumbs);
    mImageLoader = threadManager.dirWatcherImageLoader();
    connect(this,SIGNAL(startImageloader()), mImageLoader,SLOT(startWork()));
    connect( mImageLoader,SIGNAL(fertig()),this,SLOT(imageLoaderFertig()));
    connect( mImageLoader,SIGNAL(error(int)),this,SLOT(imageLoaderError(int)));
    connect( mImageLoader,SIGNAL(destroyed()),this,SLOT(onImageLoaderDestroyed()));
    connect(&mTimer,SIGNAL(timeout()),this,SLOT(timerCheckDir()));

}

void HPSDirWatcher::startFirstRun()
{

    const QStringList &list =  mDirManager.wholeDirList();
    qDebug() << Q_FUNC_INFO << list.size();
    const int size = list.size();
    for(  int i = 0 ;  i < size ; ++i ) {
        checkDir(list.at(i));
    }
}

void HPSDirWatcher::close()
{
    mTimer.stop();
    mShutDown = true;
}

void HPSDirWatcher::checkDir(const QString &dir)
{
    qDebug() << Q_FUNC_INFO << dir;
    if(!(dir == mCurrentWatched && !mIsWatching)){
        QHash<QString,QString> oldFiles = mDBHandler.fotos(dir);

        QFileInfoList fileinfos = QDir(dir).entryInfoList(QStringList() << "*.jpg"<<"*.png");
        const int sizeFileinfos = fileinfos.size();
        const int sizeOldFiles = oldFiles.size();
        QString tmpLastModi;
        for (int i = 0; i < fileinfos.size(); ++i) {
            const QFileInfo &info =  fileinfos.at(i);
            tmpLastModi = oldFiles.take(info.fileName());
            if(!tmpLastModi.isEmpty()){
                if(tmpLastModi == info.lastModified().toString()){
                    fileinfos.removeAt(i);
                    i--;
                }
            }
        }
        if(oldFiles.count()>0){
            mDBHandler.removeEntires(dir,oldFiles.keys());
            if( dir == mCurrentWatched)
                emit updateCurrent();
        }
        if(fileinfos.count() > 0){
            if( mCurrentWatched == dir)
                mIsWatching =false;
            if(!mIsImageLoaderRunning){
                prepareImageLoader(dir,fileinfos);
            }else {
                mQueue.insert(dir,fileinfos);
            }
        }
        if(sizeFileinfos == 0 && sizeOldFiles != 0 )
            mDirManager.setDirHasFiles(dir,false);
        if(sizeFileinfos!=0&&sizeOldFiles == 0)
            mDirManager.setDirHasFiles(dir,true);
    }
}

HPSDirWatcher::~HPSDirWatcher()
{
}

void HPSDirWatcher::imageLoaderFertig()
{
    qDebug() << Q_FUNC_INFO;
    const QString &dir = mImageLoader->folder();
    saveToDB(dir);
    if(dir == mCurrentWatched){
        emit updateCurrent();
        mIsWatching = true;
    }
    nextTask();
}

void HPSDirWatcher::imageLoaderError(int errorPos)
{
    qDebug() << Q_FUNC_INFO << errorPos;
}

void HPSDirWatcher::onImageLoaderDestroyed()
{
    if(mShutDown)
        deleteLater();
}

void HPSDirWatcher::prepareImageLoader(const QString &dir, const QFileInfoList &infoList)
{
    const int size = infoList.size();
    if(mThumbs.size()< size)
        mThumbs.resize(size);
    for (int i = 0; i < size; ++i) {
        HPSThumb &thumb = mThumbs[i];
        const QFileInfo &info =infoList.at(i);
        thumb.error=false;
        thumb.name = info.fileName();
        thumb.size = info.size();
        thumb.lastModi = info.lastModified().toString();
    }
    mImageLoader->setWork(dir,0,size-1,mOption.getThumbSize(),false);
    mIsImageLoaderRunning = true;
    startImageloader();
}

void HPSDirWatcher::nextTask()
{

    if(!mQueue.isEmpty()){
        if(mNextDir.isEmpty()){
            const QString&name =  mQueue.begin().key();
            prepareImageLoader(name,mQueue.take(mNextDir));
        } else {
            prepareImageLoader(mNextDir,mQueue.take(mNextDir));
        }
    }else {
        mIsImageLoaderRunning = false;
        mThumbs.clear();
    }
}

void HPSDirWatcher::setNextDir(const QString &dir)
{
    mNextDir = dir;
}

void HPSDirWatcher::saveToDB( const QString &dir)
{
    bool error = false;
    mDBHandler.startTransaction();
    const int size = mThumbs.size();

    for (int var = 0; var < size; ++var) {
        if(mShutDown){
            error = true;
            break;
        }
        const HPSThumb &thumb = mThumbs.at(var);
        if(!thumb.error) {
            //qDebug() << QApplication::applicationDirPath()+"/.thumbs/"+thumb.hash+"."+QFileInfo(thumb.name).suffix();
            if(!thumb.image.save(QApplication::applicationDirPath()+"/.thumbs/"+thumb.hash+".png")){
                qDebug()<< Q_FUNC_INFO<<"thumb konnte nicht gespeichert werden";
            }else  {
                mDBHandler.insertHash( thumb.hash, dir, thumb.name, thumb.lastModi, thumb.size,"");
            }
        }
    }
    mDBHandler.finishTransaction();
}

void HPSDirWatcher::setWatcherOn(const QString &dir)
{

    mCurrentWatched = dir;
    mIsWatching = true;
    if(!mTimer.isActive())
        mTimer.start(1000);
}

void HPSDirWatcher::timerCheckDir()
{
    qDebug() << Q_FUNC_INFO << mCurrentWatched;
    if(!mCurrentWatched.isEmpty() && mIsWatching)
        checkDir(mCurrentWatched);
}

void HPSDirWatcher::stopWatching()
{
    mCurrentWatched = "";
    mIsWatching = false;
    mTimer.stop();
}
