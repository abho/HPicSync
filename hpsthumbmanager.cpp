#include "hpsthumbmanager.h"

HPSThumbManager::HPSThumbManager(HPSOption &option, QObject *parent) :
    QObject(parent), mOption(option),mDatabaseHandler(NULL),mListWidget(NULL)
{
}


int HPSThumbManager::creatThumbs( const QString &cDir,const bool subDirs,const bool view)
{
    QStringList allSubDirs;
    if(!mOption.getOrdner().contains(cDir))
        allSubDirs << cDir;
    if(subDirs)
        subDirsFrom(cDir,allSubDirs);
    qDebug() << allSubDirs;
    if(!allSubDirs.isEmpty()){
        mOption.appendCreateThumbDir(allSubDirs);
        makeThumbsAndView(cDir,view);
    }
    return allSubDirs.size();
}
void HPSThumbManager::fotosReady(int pos, int count)
{
    qDebug() << Q_FUNC_INFO << pos << count;
    if(count !=0 ){
        for(int i = pos;i<pos+count;i++){
            QListWidgetItem *item= new QListWidgetItem();

            QImage *img =&this->mThumbs[i].image;
            item->setData(Qt::DisplayRole,mThumbs.at(i).name);
            item->setData(Qt::DecorationRole,qVariantFromValue((void *) img));
            //img =&this->fotos[i];
            item->setData(Qt::UserRole+2,qVariantFromValue((void *) img));
            item->setData(Qt::UserRole+1,i);
            item->setData(Qt::UserRole+3, mOption.getThumbSize());
            mListWidget->addItem(item);
        }
    }


}

void HPSThumbManager::threadClear()
{
    QThread *thread =(QThread*)sender();
    mThreads.remove(thread);
    thread->deleteLater();
}

void HPSThumbManager::fertigTime()
{
    qDebug() << "fertig" << mTimer.elapsed();

}

bool HPSThumbManager::allThreadsClose()
{
    return mThreads.isEmpty();
}

void HPSThumbManager::closeAllThreads()
{
    QMapIterator<QThread *, HPSWorkerClass*> i(mThreads);
    while (i.hasNext()) {
        i.next();
        this->connect(i.key(),SIGNAL(destroyed()),this,SLOT(checkIfAllClose()));
        i.value()->beenden();
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

void HPSThumbManager::setDatenBankHandler(HPSDBHandler *handler)
{
    mDatabaseHandler = handler;
}

void HPSThumbManager::hashesReady()
{
    qDebug() << Q_FUNC_INFO << "hashesReady";
    mOption.removeCreateThumbDir( mCurrentDir);
}



void HPSThumbManager::reset()
{
    mThumbsLoaded = 0;
    mThumbs.clear();
    mCountError  = 0;
}

void  HPSThumbManager::makeThumbsAndView( const QString &cDir, const bool withView)
{
    qDebug() << Q_FUNC_INFO ;
    qDebug() << withView;

    mTimer.start();
    mCurrentDir =cDir;
    reset();
    int number = QThread::idealThreadCount();
    qDebug() << "number"<< number;
    QDir dir( mCurrentDir);
    QFileInfoList fileNames = dir.entryInfoList(QStringList() << "*.jpg"<<"*.png");
    const int size= fileNames.size();
    qDebug() <<"fileNames.size"<<size;
    if(size>0){
        mThumbs.resize(size);
        HPSThumb thumb;

        for (int var = 0; var <size; ++var) {
            const QFileInfo &fileInfo =fileNames.at(var);
            thumb.name =fileInfo.fileName();
            thumb.lastModi =fileInfo.lastModified().toString();
            thumb.size = fileInfo.size();
            thumb.suffix = fileInfo.suffix();
            mThumbs[var]=thumb;
        }
        HPSImageLoader::setThumbVector( &mThumbs);
        HPSImageLoader::setFolder( mCurrentDir);

        int partSize= size/number;
        qDebug() << "partSize" << partSize;
        //mFileNames[9]="muh";
        int pos=0, end=-1;
        for (int  i = 0; i< number;i++){
            pos =end+1;
            if(i==number-1)
                end=size-1;
            else
                end+=partSize;
            qDebug() << "pos"<<pos <<"end"<< end;
            HPSImageLoader *imageLoader = new HPSImageLoader(mMutex,pos,end, mOption.getThumbSize());
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
    }
    emit startThumbCreation( mCurrentDir,size);
    if( size == 0)
        emit thumbsReady(0);
}
bool HPSThumbManager::dirReady(const QString &dir)
{
    return !mOption.createThumbDirs().contains(dir);
}

void HPSThumbManager::setListWidget(QListWidget *listWidget)
{
    mListWidget = listWidget;
}

void HPSThumbManager::fotoReady()
{qDebug() << Q_FUNC_INFO ;
    mThumbsLoaded++;
    emit thumbsReady( mThumbsLoaded);
    if(mThumbsLoaded == mThumbs.size())
        saveHashes();
}

void HPSThumbManager::subDirsFrom(const QString &dir, QStringList &dirs)
{
    QDir cDir(dir);
    QStringList list = cDir.entryList(QStringList()<< "*",QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot |QDir::Hidden);
#ifdef Q_OS_WIN32
    list.removeOne("Thumbs.db");
#endif
    const int size = list.size();
    for (int var = 0; var < size; ++var) {
        subDirsFrom(QString(dir+"/"+list.at(var)),dirs);
        if(!mOption.getOrdner().contains(dir+"/"+list.at(var)))
            dirs.append(dir+"/"+list.at(var));
    }

}

int HPSThumbManager::startWork()
{

    if(!mOption.createThumbDirs().isEmpty()){
        if(mOption.createThumbDirs().contains( mOption.getComboBoxCurrentDir()))
            makeThumbsAndView( mOption.getComboBoxCurrentDir(),true);
        else
            makeThumbsAndView(mOption.createThumbDirs().first(),false);
    }
    return mOption.createThumbDirs().size();
}
