#include "hpsdirlister.h"




HPSDirLister::HPSDirLister(HPSKnotDirModel &model,QObject *parent) :
    QObject(parent), mDirModel(model),mIsRunning(false),mShutDown(false)
{

}

HPSDirLister::~HPSDirLister(){
    qDebug() <<Q_FUNC_INFO << "tot";
}

void HPSDirLister::startWork(const QString &path)
{
    mIsRunning = true;
    qDebug() << "start" << path;
    timer.start();

    subDirsFrom(path);
    qDebug() << "ready " << timer.elapsed();
    if(mShutDown)
        emit workDone();
    else{
        emit workSemiDone();
        deleteLater();
    }
    mIsRunning = false;
}

void HPSDirLister::subDirsFrom(const QString &dir)
{
    if(!mShutDown){
        if(!mDirModel.contains(dir)){
            emit dirDone(dir);
        }
    }

    mDir.setPath(dir);
    QStringList list = mDir.entryList(QStringList()<< "*",QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    foreach (const QString &str, list) {
        if(!mShutDown){
            subDirsFrom(QString(dir+"/"+QString::fromLatin1(str.toLatin1())));
        } else {
            break;
        }
    }

}

void HPSDirLister::close()
{

    if( mIsRunning)
        mShutDown = true;
    else
        deleteLater();
}



QStringList HPSDirLister::dirs()
{
    return mList;
}






