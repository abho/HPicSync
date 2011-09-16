#include "hpsdirchecker.h"

HPSDirChecker::HPSDirChecker(HPSThreadManager &threadManager,HPSOption &option,HPSDBHandler *dbHandler,QObject *parent) :
    QObject(parent),mThreadManager(threadManager),mOption(option),mDatabaseHandler(dbHandler),
    mShutDown(false),mIsRunning(false),mIsWorking(false)
{
}
HPSDirChecker::~HPSDirChecker(){
    qDebug() << Q_FUNC_INFO << "tot";
}

void HPSDirChecker::close()
{
    if( mIsRunning)
        mShutDown =true;
    else
        deleteLater();
}

void HPSDirChecker::checkDir(QString dir)
{

    mDirQueue.append(dir);
    if(!mIsWorking){
        mIsWorking = true;
        makeThumbsAndView(dir,false);

    }
}

void HPSDirChecker::startWork(const QString &cDir, const bool withView)
{

    qDebug() << Q_FUNC_INFO << cDir ;
    QVector<HPSImageLoader*> &imageLoaders =  mThreadManager.imageLoaders();

    QFileInfoList fileInfos = QDir(cDir).entryInfoList(QStringList() << "*.jpg"<<"*.png");
    const int size= fileInfos.size();
    if(size>0){
        mThumbs.resize(size);
        HPSThumb thumb;

        for (int var = 0; var <size; ++var) {
            const QFileInfo &fileInfo =fileNames.at(var);
            thumb.name =fileInfo.fileName();
            thumb.lastModi =fileInfo.lastModified().toString();
            thumb.size = fileInfo.size();
            mThumbs[var]=thumb;
        }
        HPSImageLoader::setThumbVector( &mThumbs);
        HPSImageLoader::setFolder( mCurrentDir);

        int partSize= size/number;
        //qDebug() << "partSize" << partSize;
        //mFileNames[9]="muh";
        int pos=0, end=-1;
        for (int  i = 0; i< number;i++){
            pos =end+1;
            if(i==number-1)
                end=size-1;
            else
                end+=partSize;
            //qDebug() << "pos"<<pos <<"end"<< end;
            HPSImageLoader *imageLoader = new HPSImageLoader(pos,end, mOption.getThumbSize());
            QThread *thread = new QThread();
            mThreads.insert(thread,imageLoader);
            imageLoader->moveToThread(thread);

            connect(imageLoader,SIGNAL(destroyed()),thread,SLOT(quit()));
            connect(thread,SIGNAL(finished()),this,SLOT(threadClear()));

            if(withView) {
                connect(thread,SIGNAL(started()),imageLoader,SLOT(startWithView()));
                connect(imageLoader,SIGNAL(ready(int,int)),this,SLOT(fotosReady(int,int)));
            } else {
                connect(thread,SIGNAL(started()),imageLoader,SLOT(startWithoutView()));
            }
            connect(imageLoader,SIGNAL(fotosReady()),this,SLOT(fotoReady()));
            connect(imageLoader,SIGNAL(fertig()),this,SLOT(fertigTime()));
            connect(imageLoader,SIGNAL(error(int)),this,SLOT(getError(int)));
            thread->start();
        }

    } else {
        workReady(cDir);
        nextWork();
    }
}

void HPSDirChecker::workReady(const QString &dir)
{
    mDirQueue.removeOne(dir);
    emit dirChecked(dir);

}

void HPSDirChecker::nextWork()
{

    if(!mDirQueue.isEmpty()){

        makeThumbsAndView(mDirQueue.first()mOption.createThumbDirs().first(),false);
    }else
        mIsWorking = false;


}
/*

void HPSThumbManager::creatThumbs( const QStringList &cDirs)
{

    //emit startCreation();
  //  mOption.appendCreateThumbDir(cDirs);
   // makeThumbsAndView(cDirs.first(),view);

}
void HPSThumbManager::creatThumbs(const QString &cDir){
    //emit startCreation();
    mOption.addCreateThumbDir(cDir);
    qDebug() << Q_FUNC_INFO << cDir;
    if(mOption.createThumbDirs().size() == 1)
        makeThumbsAndView(cDir,view);

}


void HPSThumbManager::fotosReady(int pos, int count)
{
    qDebug() << Q_FUNC_INFO << pos << count;
    if(count !=0 ){
        for(int i = pos;i<pos+count;i++){
            QListWidgetItem *item= new QListWidgetItem();

            QImage *img =&this->mThumbs[i].image;
            item->setData(Qt::DisplayRole,mThumbs.at(i).name);
            item->setData(Qt::DecorationRole,QPixmap::fromImage(*img));

            mListWidget->addItem(item);
        }
    }
}
void HPSThumbManager::checkIfAllClose()
{
    if(mThreads.empty())
        emit allThreadsDestroyed();
}

void HPSThumbManager::getError(int error)
{
    mCountError++;
    qDebug() << Q_FUNC_INFO;
    qDebug()<< "error while creating thumbnail of" << mThumbs.at(error).name;
}

void HPSThumbManager::saveHashes()
{
    HPSHashSaver *hashSaver = new HPSHashSaver(mDatabaseHandler, mThumbs, mCurrentDir);
    QThread *thread = new QThread();
    mThreads.insert(thread,hashSaver);
    hashSaver->moveToThread(thread);
    connect(thread,SIGNAL(started()),hashSaver,SLOT(start()));
    connect(hashSaver,SIGNAL(destroyed()),thread,SLOT(quit()));
    connect(thread,SIGNAL(finished()),this,SLOT(threadClear()));
    connect(hashSaver,SIGNAL(finish()),this,SLOT(hashesReady()));
    thread->start();
}
void HPSThumbManager::hashesReady()
{
    //qDebug() << Q_FUNC_INFO << "hashesReady";
    nextWork();
}

void HPSThumbManager::reset()
{
    mThumbsLoaded = 0;
    mThumbs.clear();
    mCountError  = 0;
}


void HPSThumbManager::fotoReady()
{
    //qDebug() << Q_FUNC_INFO ;
    mThumbsLoaded++;
    emit thumbsReady( mThumbsLoaded);
    if(mThumbsLoaded == mThumbs.size())
        saveHashes();
}


int HPSThumbManager::workCount()
{
    //qDebug() << Q_FUNC_INFO << mOption.createThumbDirs().size();
    return mOption.createThumbDirs().size();
}

