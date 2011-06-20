#include "hpsthumbmanager.h"

HPSThumbManager::HPSThumbManager(HPSOption &option, QObject *parent) :
    QObject(parent), mOption(option),mDatabaseHandler(NULL)
{
}


void HPSThumbManager::creatThumbs( const QStringList &cDirs,const bool view)
{
emit startCreation();
    mOption.appendCreateThumbDir(cDirs);
    makeThumbsAndView(cDirs.first(),view);

}

void HPSThumbManager::creatThumbs(const QString &cDir,const bool view){
emit startCreation();
    mOption.addCreateThumbDir(cDir);
    qDebug() << Q_FUNC_INFO << mOption.createThumbDirs().size();
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
    nextWork();
}



void HPSThumbManager::reset()
{
    mThumbsLoaded = 0;
    mThumbs.clear();
    mCountError  = 0;
}

void  HPSThumbManager::makeThumbsAndView( const QString &cDir, const bool withView)
{
    if (withView)
        mListWidget->clear();
    qDebug() << Q_FUNC_INFO << cDir ;
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
    emit startThumbCreation( mCurrentDir,size);
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
        nextWork();
    }

}
bool HPSThumbManager::dirReady(const QString &dir)
{qDebug() << !mOption.createThumbDirs().contains(dir);
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
        if(!mOption.dirs().contains(dir+"/"+list.at(var))&&!mOption.createThumbDirs().contains(dir+"/"+list.at(var)))
            dirs.append(dir+"/"+list.at(var));
    }

}

bool HPSThumbManager::startWork()
{
    qDebug() << Q_FUNC_INFO << mOption.createThumbDirs();
    if(!mOption.createThumbDirs().isEmpty()){
        emit startCreation();
        if(mOption.createThumbDirs().contains( mOption.getComboBoxCurrentDir()))
            makeThumbsAndView( mOption.getComboBoxCurrentDir(),true);
        else
            makeThumbsAndView(mOption.createThumbDirs().first(),false);
        return true;
    }  else
        return false;
}

int HPSThumbManager::workCount()
{qDebug() << Q_FUNC_INFO << mOption.createThumbDirs().size();
    return mOption.createThumbDirs().size();
}

void HPSThumbManager::loadThumbs(const QString &dir)
{
    qDebug() << Q_FUNC_INFO << dir;
    if(mDatabaseHandler!=NULL){
        mListWidget->clear();
        QList<QStringList> list;
        list = mDatabaseHandler->hashPaths(dir);
        makeView(list);
        qDebug() << Q_FUNC_INFO << mDatabaseHandler->hashPaths(dir);
    } else {
        qDebug() << "databasehandler erst inizalisieren.";
    }
}

void HPSThumbManager::makeView(const QList<QStringList> &list)
{
    const QStringList &hashList = list.at(0);
    const QStringList &nameList = list.at(1);
    const int size = hashList.size();
    QPixmap pix;
    for (int var = 0; var < size; ++var) {
        QListWidgetItem *item= new QListWidgetItem();
        if(pix.load(QApplication::applicationDirPath()+"/.thumbs/"+hashList.at(var)+".png")){

            item->setData(Qt::DisplayRole,nameList.at(var));
            item->setData(Qt::DecorationRole,pix);
        } else {
            qDebug() << "fehler bei laden von "<<QApplication::applicationDirPath()+"/.thumbs/"+hashList.at(var)+".jpg";
        }
        //img =&this->fotos[i];
        mListWidget->addItem(item);
    }
}

void HPSThumbManager::nextWork()
{
    mOption.removeCreateThumbDir( mCurrentDir);
    if(!mOption.createThumbDirs().isEmpty())
        makeThumbsAndView(mOption.createThumbDirs().first(),false);
    else
        emit creationReady();


}
