#include "hpsthumbmanager.h"

HPSThumbManager::HPSThumbManager(QObject *parent) :
    QObject(parent), mDatabaseHandler(NULL),mListWidget(NULL)
{
}

int HPSThumbManager::creatThumbsAndView(const int thumbSize,const QString &cDir, QListWidget *listWidgte)
{
    return makeThumbsAndView(thumbSize,cDir,listWidgte);
}

void HPSThumbManager::fotosReady(int pos, int count, const QString &str)
{
    qDebug() << Q_FUNC_INFO << pos << count << str;
    mThumbsLoaded +=count;
    if(count !=0 ){
        for(int i = pos;i<pos+count;i++){
            QListWidgetItem *item= new QListWidgetItem();

            QImage *img =&this->mThumbs[i].image;
            item->setData(Qt::DisplayRole,img->text("name"));
            item->setData(Qt::DecorationRole,qVariantFromValue((void *) img));
            //img =&this->fotos[i];
            item->setData(Qt::UserRole+2,qVariantFromValue((void *) img));
            item->setData(Qt::UserRole+1,i);
            item->setData(Qt::UserRole+3, mCurrentThumbSize);
            item->setCheckState(Qt::Unchecked);
            mListWidget->addItem(item);
        }
        emit thumsLoaded( mThumbsLoaded+mCountError);
    }
    if(mThumbsLoaded+mCountError==mThumbs.size())
        saveHashes();

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
}

int HPSThumbManager::creatThumbs(const int thumbSize, const QString &cDir)
{

    return makeThumbsAndView(thumbSize,cDir,NULL);
}

void HPSThumbManager::reset()
{
    mThumbsLoaded = 0;
    mThumbs.clear();
    mCountError  = 0;
}

int HPSThumbManager::makeThumbsAndView(const int thumbSize, const QString &cDir, QListWidget *listWidgte)
{
    qDebug() << Q_FUNC_INFO << listWidgte;

    mTimer.start();
    mCurrentDir =cDir;
    if(listWidgte!=NULL)
        mListWidget = listWidgte;
    mCurrentThumbSize = thumbSize;
    reset();
    int number = QThread::idealThreadCount();
    qDebug() << "number"<< number;
    QDir dir( mCurrentDir);
    QStringList fileNames = dir.entryList(QStringList() << "*.jpg");
    const int size= fileNames.size();
    qDebug() <<"fileNames.size"<<size;
    mThumbs.resize(size);
    for (int var = 0; var <size; ++var) {
        mThumbs[var]=HPSThumb(fileNames.at(var));
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
        HPSImageLoader *imageLoader = new HPSImageLoader(mMutex,pos,end, mCurrentThumbSize);
        QThread *thread = new QThread();
        mThreads.insert(thread,imageLoader);
        imageLoader->moveToThread(thread);

        connect(imageLoader,SIGNAL(destroyed()),thread,SLOT(quit()));
        connect(thread,SIGNAL(finished()),this,SLOT(threadClear()));

        if(listWidgte!=NULL) {
            connect(thread,SIGNAL(started()),imageLoader,SLOT(startWithView()));
            connect(imageLoader,SIGNAL(ready(int,int,QString)),this,SLOT(fotosReady(int,int,QString)));
        } else {
            connect(thread,SIGNAL(started()),imageLoader,SLOT(startWithoutView()));
        }

        connect(imageLoader,SIGNAL(fertig()),this,SLOT(fertigTime()));
        connect(imageLoader,SIGNAL(error(int)),this,SLOT(getError(int)));
        thread->start();
    }
    return size;
}
