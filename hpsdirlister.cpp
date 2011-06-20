#include "hpsdirlister.h"

HPSDirLister::HPSDirLister(HPSOption &option,const QStringList &saversFiles,const QString &path,QObject *parent) :
    QObject(parent),mOption(option),mPath(path),mEx(false)
{
    mSaver.setList(saversFiles);
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
    if(!mOption.dirs().contains(dir)){
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
            //mSaver.add(dir);
            //if(!mOption.dirs().contains(dir))
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

QStringList HPSDirLister::saverDirs()
{
    return mSaver.list();
}

QStringList HPSDirLister::dirs()
{
    return mList;
}
