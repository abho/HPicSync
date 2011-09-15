#include "hpsdirlister.h"




HPSDirLister::HPSDirLister(HPSKnotDirModel &model,QObject *parent) :
    QObject(parent), mDirModel(model),mEx(false)
{

}

HPSDirLister::~HPSDirLister(){
    qDebug() <<Q_FUNC_INFO << "tot";
}

void HPSDirLister::startWork(const QString &path)
{
    mEx = false;
    qDebug() << "start" << path;
    timer.start();

    subDirsFrom(path);
    qDebug() << "ready " << timer.elapsed();
    if(!mEx)
        emit workDone();
    else
        emit workSemiDone();
}

void HPSDirLister::subDirsFrom(const QString &dir)
{
    if(!mEx){
        if(!mDirModel.contains(dir)){
            emit dirDone(dir);
        }
    }

    mDir.setPath(dir);
    QStringList list = mDir.entryList(QStringList()<< "*",QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    foreach (const QString &str, list) {
        if(!mEx){
            subDirsFrom(QString(dir+"/"+QString::fromLatin1(str.toLatin1())));
        } else {
            break;
        }
    }

}

void HPSDirLister::kill()
{
    mEx = true;
}



QStringList HPSDirLister::dirs()
{
    return mList;
}






