#include "hpsdirchecker.h"
#include "hpsdirlister.h"

HPSDirChecker::HPSDirChecker(QVector<QListWidgetItem*> &vec,HPSThreadManager &threadManager,HPSOption &option,HPSDBHandler &dbHandler,QObject *parent) :
    QObject(parent),mTmpListWidgetItems(vec),mThreadManager(threadManager),mOption(option),
    mDatabaseHandler(dbHandler),mCountDestroyedImageLoader(0),
    mShutDown(false),mIsRunning(false),mIsWorking(false)
{
    mThreadManager.initImagerLoaders(mThumbs);
    QVector<HPSImageLoader*> &imageLoaders =  mThreadManager.imageLoaders();
    mMaxDestroyedImageLoaders = imageLoaders.size();
    HPSImageLoader *imageLoader;
    for (int i = 0; i < imageLoaders.size(); ++i) {
        imageLoader = imageLoaders[i];
        connect(this,SIGNAL(startImageloaders()),imageLoader,SLOT(startWork()));
        connect(imageLoader,SIGNAL(ready(int,int)),this,SLOT(fotosReady(int,int)));
        connect(imageLoader,SIGNAL(fertig()),this,SLOT(imageLoaderFertig()));
        connect(imageLoader,SIGNAL(error(int)),this,SLOT(imageLoaderError(int)));
        connect(imageLoader,SIGNAL(destroyed()),this,SLOT(onImageLoaderDestroyed()));
    }

}
HPSDirChecker::~HPSDirChecker(){
    qDebug() << Q_FUNC_INFO << "tot";
}

void HPSDirChecker::close()
{
    mShutDown =true;
}

void HPSDirChecker::checkDir(QString dir)
{
    mDirQueue.append(dir);
    if(!mIsWorking){
        mIsWorking = true;
        startWork(dir,false);
    }
}

void HPSDirChecker::startWork(const QString &cDir, const bool withView)
{
    if(!mShutDown){
        mCurrentDir = cDir;
        emit startCheckOn(cDir);
        QVector<HPSImageLoader*> &imageLoaders =  mThreadManager.imageLoaders();

        QFileInfoList fileInfos = QDir(cDir).entryInfoList(QStringList() << "*.jpg"<<"*.png");
        const int size= fileInfos.size();
        if(size>0){
           // qDebug() << Q_FUNC_INFO << cDir << size << mShutDown << mIsRunning;
            mThumbs.resize(size);
            HPSThumb thumb;
            for (int var = 0; var <size; ++var) {
                const QFileInfo &fileInfo =fileInfos.at(var);
                thumb.name =fileInfo.fileName();
                thumb.lastModi =fileInfo.lastModified().toString();
                thumb.size = fileInfo.size();
                mThumbs[var]=thumb;
            }

            int number = imageLoaders.size();
            int partSize= size/number;
            int pos=0, end=-1;
            mCountImageLoader.first = number;
            mCountImageLoader.second = 0;
            for (int  i = 0; i< number;i++){
                pos =end+1;
                if(i==number-1)
                    end=size-1;
                else
                    end+=partSize;
                imageLoaders[i]->setWork(cDir,pos,end,mOption.getThumbSize(),withView);
            }
            emit startImageloaders();
        } else {
          //  qDebug() << Q_FUNC_INFO << cDir << "keine bilder" << mIsRunning;
            workReady(cDir,false);
            nextWork();
        }
    }
}

void HPSDirChecker::workReady(const QString &dir,bool hasFiles)
{
    if(!mShutDown){
        mDirQueue.removeOne(dir);
        emit dirChecked(dir,hasFiles);
    }
}

void HPSDirChecker::nextWork()
{

    if(!mDirQueue.isEmpty()){
        startWork(mDirQueue.first(),false);
    }else{
        mCurrentDir ="";
        mIsWorking = false;
        if(!mShutDown&&!mThreadManager.dirLister()->isRunning())
            mOption.setDirFromDirlister("");
        emit finished();
    }
}

void HPSDirChecker::imageLoaderFertig()
{
    qDebug() << Q_FUNC_INFO;
    mCountImageLoader.second++;
    if( mCountImageLoader.first == mCountImageLoader.second){
        if(!saveThumbsToDB(mCurrentDir))
            workReady(mCurrentDir,true);
        if(!mShutDown)
            nextWork();
        else
            deleteLater();
    }
}

void HPSDirChecker::imageLoaderError(int errorPos)
{
    qDebug() << Q_FUNC_INFO << errorPos;
}

void HPSDirChecker::fotosReady(int pos, int count)
{
    qDebug() << Q_FUNC_INFO << pos << count;
    if(count !=0 ){
        QListWidgetItem *item;
        for(int i = pos;i<pos+count;i++){
            item = new QListWidgetItem();
            QImage *img =&this->mThumbs[i].image;
            item->setData(Qt::DisplayRole,mThumbs.at(i).name);
            item->setData(Qt::DecorationRole,QPixmap::fromImage(*img));
            mTmpListWidgetItems[i] = item;
        }
        emit newItemListWidgtesReady(pos,count);
    }
}

bool HPSDirChecker::saveThumbsToDB(const QString &dir)
{
    QElapsedTimer timer;
    timer.start();
    bool error = false;
    mDatabaseHandler.startTransaction();
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
                mDatabaseHandler.insertHash( thumb.hash, dir, thumb.name, thumb.lastModi, thumb.size,"");
            }
        }
    }
    mDatabaseHandler.finishTransaction();
    return error;
}

void HPSDirChecker::onImageLoaderDestroyed()
{
    qDebug() << Q_FUNC_INFO;
    mCountDestroyedImageLoader++;
    if(mCountDestroyedImageLoader == mMaxDestroyedImageLoaders){
        if( mShutDown)
            deleteLater();
    }
}
