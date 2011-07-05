#include "hpsdirlister.h"




HPSDirLister::HPSDirLister(HPSKnotDirModel &model, const QString &path,QObject *parent) :
    QObject(parent), mDirModel(model),mPath(path),mEx(false)
{
    //mSaver.setList(saversFiles);

}

HPSDirLister::~HPSDirLister(){
    qDebug() <<Q_FUNC_INFO << "tot";
}

void HPSDirLister::startWork()
{
    mEx = false;
    qDebug() << "start" << mPath;
    timer.start();

    subDirsFrom(mPath);
    qDebug() << "ready " << timer.elapsed();
    if(!mEx)
        emit workDone();
    else
        emit workSemiDone();
}

void HPSDirLister::subDirsFrom(const QString &dir)
{
    //qDebug() << Q_FUNC_INFO << dir.toUtf8();

    if(!mEx){
        if(!mDirModel.contains(dir)){
            //mDirModel.add(dir);
           // mList.append(dir);
            emit dirDone(dir);
        }
    }

    mDir.setPath(dir);
    QStringList list = mDir.entryList(QStringList()<< "*",QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    foreach (const QString str, list) {
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






