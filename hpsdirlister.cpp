#include "hpsdirlister.h"




HPSDirLister::HPSDirLister(HPSKnotDirModel &model, const QString &path,QObject *parent) :
    QObject(parent), mSaver(model),mPath(path),mEx(false)
{
    //mSaver.setList(saversFiles);

}

HPSDirLister::~HPSDirLister(){
    qDebug() <<Q_FUNC_INFO << "tot";
}

void HPSDirLister::startWork()
{
    mEx = false;
    qDebug() << "start";
    subDirsFrom(mPath);
    if(!mEx)
        emit workDone();
    else
        emit workSemiDone();
}

void HPSDirLister::subDirsFrom(const QString &dir)
{
    if(!mSaver.contains(dir)){
        mDir.setPath(dir);
        QStringList list = mDir.entryList(QStringList()<< "*",QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot);
        foreach (const QString str, list) {
            if(!mEx){
                subDirsFrom(QString(dir+"/"+str));
            } else {
                break;
            }
        }
        if(!mEx){
            mSaver.add(dir);
            mList.append(dir);
            if(mList.size()%10 == 0)
                chunkDone();
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






